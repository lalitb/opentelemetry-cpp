#include "stress.h"
#include <chrono>
#include <fstream>  // For Linux memory stats
#include <iostream>
#include <sstream>
#include <thread>

#ifdef _WIN32
#  include <psapi.h>
#  include <windows.h>
#else
#  include <unistd.h>
#endif

std::atomic<bool> STOP(false);

StressTest::StressTest(std::function<void()> func, std::size_t num_threads)
    : func_(std::move(func)), num_threads_(num_threads)
{}

void StressTest::run()
{
  std::cout << "Number of threads: " << num_threads_ << "\n\n";

  std::vector<std::thread> threads;

  // Initialize stats_ vector
  for (std::size_t i = 0; i < num_threads_; ++i)
  {
    stats_.emplace_back(std::make_unique<WorkerStats>());
  }

  auto stats_collector = [this]() {
    using namespace std::chrono;
    auto last_collect_time   = steady_clock::now();
    uint64_t total_count_old = 0;

    while (!STOP.load(std::memory_order_acquire))
    {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      auto current_time = steady_clock::now();
      auto elapsed      = duration_cast<seconds>(current_time - last_collect_time).count();

      if (elapsed >= SLIDING_WINDOW_SIZE)
      {
        uint64_t total_count   = calculate_total_count();
        uint64_t current_count = total_count - total_count_old;
        total_count_old        = total_count;
        uint64_t throughput    = current_count / elapsed;

        std::cout << "Throughput: " << format_number(throughput) << " iterations/sec\n";

        print_memory_usage();
        last_collect_time = steady_clock::now();
      }
    }
  };

  threads.emplace_back(stats_collector);

  for (std::size_t i = 0; i < num_threads_; ++i)
  {
    threads.emplace_back(worker_thread, i, this);
  }

  for (auto &thread : threads)
  {
    if (thread.joinable())
    {
      thread.join();
    }
  }

  STOP.store(false, std::memory_order_release);
}

void StressTest::signal_handler()
{
  STOP.store(true, std::memory_order_release);
}

uint64_t StressTest::calculate_total_count() const
{
  uint64_t total_count = 0;
  for (const auto &stat : stats_)
  {
    total_count += stat->count.load(std::memory_order_relaxed);  // Dereference unique_ptr
  }
  return total_count;
}

void StressTest::worker_thread(std::size_t index, StressTest *self)
{
  while (!STOP.load(std::memory_order_acquire))
  {
    self->func_();
    self->stats_[index]->count.fetch_add(1, std::memory_order_relaxed);  // Dereference unique_ptr
  }
}

std::string StressTest::format_number(uint64_t number)
{
  std::ostringstream oss;
  oss.imbue(std::locale(""));
  oss << number;
  return oss.str();
}

void StressTest::print_memory_usage() const
{
#ifdef _WIN32
  PROCESS_MEMORY_COUNTERS_EX pmc;
  if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc)))
  {
    std::cout << "Memory Usage: " << format_number(pmc.WorkingSetSize / 1024) << " KB\n";
  }
#else
  std::ifstream statm("/proc/self/statm");
  if (statm.is_open())
  {
    uint64_t vm_pages, rss_pages;
    statm >> vm_pages >> rss_pages;
    long page_size_kb  = sysconf(_SC_PAGE_SIZE) / 1024;  // Convert to KB
    uint64_t memory_kb = rss_pages * page_size_kb;
    std::cout << "Memory Usage: " << format_number(memory_kb) << " KB\n";
  }
#endif
}
