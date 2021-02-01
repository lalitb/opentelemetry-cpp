#pragma once

#include "opentelemetry/sdk/common/circular_buffer.h"
#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"

#include <atomic>
#include <condition_variable>
#include <thread>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{

namespace trace
{

/**
 * This is an implementation of the SpanProcessor which creates batches of finished spans and passes
 * the export-friendly span data representations to the configured SpanExporter.
 */
class BatchSpanProcessor : public SpanProcessor
{
public:
  /**
   * Creates a batch span processor by configuring the specified exporter and other parameters
   * as per the official, language-agnostic opentelemetry specs.
   *
   * @param exporter - The backend exporter to pass the ended spans to
   * @param max_queue_size -  The maximum buffer/queue size. After the size is reached, spans are
   * dropped.
   * @param schedule_delay_millis - The time interval between two consecutive exports.
   * @param max_export_batch_size - The maximum batch size of every export. It must be smaller or
   * equal to max_queue_size
   */
  explicit BatchSpanProcessor(
      std::unique_ptr<SpanExporter> &&exporter,
      const size_t max_queue_size                           = 2048,
      const std::chrono::milliseconds schedule_delay_millis = std::chrono::milliseconds(5000),
      const size_t max_export_batch_size                    = 512);

  /**
   * Requests a Recordable(Span) from the configured exporter.
   *
   * @return A recordable generated by the backend exporter
   */
  std::unique_ptr<Recordable> MakeRecordable() noexcept override;

  /**
   * Called when a span is started.
   *
   * NOTE: This method is a no-op.
   *
   * @param span - The span that just started
   * @param parent_context - The parent context of the span that just started
   */
  void OnStart(Recordable &span,
               const opentelemetry::trace::SpanContext &parent_context =
                   opentelemetry::trace::SpanContext::GetInvalid()) noexcept override;

  /**
   * Called when a span ends.
   *
   * @param span - A recordable for a span that just ended
   */
  void OnEnd(std::unique_ptr<Recordable> &&span) noexcept override;

  /**
   * Export all ended spans that have not been exported yet.
   *
   * NOTE: Timeout functionality not supported yet.
   */
  bool ForceFlush(
      std::chrono::microseconds timeout = std::chrono::microseconds::max()) noexcept override;

  /**
   * Shuts down the processor and does any cleanup required. Completely drains the buffer/queue of
   * all its ended spans and passes them to the exporter. Any subsequent calls to OnStart, OnEnd,
   * ForceFlush or Shutdown will return immediately without doing anything.
   *
   * NOTE: Timeout functionality not supported yet.
   */
  bool Shutdown(
      std::chrono::microseconds timeout = std::chrono::microseconds::max()) noexcept override;

  /**
   * Class destructor which invokes the Shutdown() method. The Shutdown() method is supposed to be
   * invoked when the Tracer is shutdown (as per other languages), but the C++ Tracer only takes
   * shared ownership of the processor, and thus doesn't call Shutdown (as the processor might be
   * shared with other Tracers).
   */
  ~BatchSpanProcessor();

private:
  /**
   * The background routine performed by the worker thread.
   */
  void DoBackgroundWork();

  /**
   * Exports all ended spans to the configured exporter.
   *
   * @param was_force_flush_called - A flag to check if the current export is the result
   *                                 of a call to ForceFlush method. If true, then we have to
   *                                 notify the main thread to wake it up in the ForceFlush
   *                                 method.
   */
  void Export(const bool was_for_flush_called);

  /**
   * Called when Shutdown() is invoked. Completely drains the queue of all its ended spans and
   * passes them to the exporter.
   */
  void DrainQueue();

  /* The configured backend exporter */
  std::unique_ptr<SpanExporter> exporter_;

  /* Configurable parameters as per the official specs */
  const size_t max_queue_size_;
  const std::chrono::milliseconds schedule_delay_millis_;
  const size_t max_export_batch_size_;

  /* Synchronization primitives */
  std::condition_variable cv_, force_flush_cv_;
  std::mutex cv_m_, force_flush_cv_m_;

  /* The buffer/queue to which the ended spans are added */
  common::CircularBuffer<Recordable> buffer_;

  /* Important boolean flags to handle the workflow of the processor */
  std::atomic<bool> is_shutdown_{false};
  std::atomic<bool> is_force_flush_{false};
  std::atomic<bool> is_force_flush_notified_{false};

  /* The background worker thread */
  std::thread worker_thread_;
};

}  // namespace trace
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
