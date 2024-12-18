#ifndef STRESS_H
#define STRESS_H

#include <atomic>
#include <cstddef>
#include <functional>
#include <memory>
#include <vector>

// Define sliding window size in seconds
constexpr std::size_t SLIDING_WINDOW_SIZE = 2;

// Extern declaration for the STOP flag
extern std::atomic<bool> STOP;

// Declaration of WorkerStats
struct WorkerStats
{
  std::atomic<uint64_t> count{0};
  WorkerStats() : count(0) {}  // Initialize count to 0
};

// Declaration of StressTest class
class StressTest
{
public:
  StressTest(std::function<void()> func, std::size_t num_threads);
  void run();
  static void signal_handler();

private:
  std::function<void()> func_;
  std::size_t num_threads_;
  std::vector<std::unique_ptr<WorkerStats>> stats_;  // Use unique_ptr for WorkerStats

  uint64_t calculate_total_count() const;
  static void worker_thread(std::size_t index, StressTest *self);
  static std::string format_number(uint64_t number);
  void print_memory_usage() const;
};

#endif  // THROUGHPUT_H
