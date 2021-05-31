// Please DONOT touch this file.
// Any changes done here would be overwritten during release/build.

#ifdef ENABLE_METRICS_PREVIEW
#  include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace exporter
{
namespace prometheus
{
namespace version
{
const int MAJOR_VERSION     = 0;
const int MINOR_VERSION     = 7;
const int PATCH_VERSION     = 0;
const char *PRE_RELEASE     = "";
const char *BUILD_METADATA  = "";
const int COUNT_NEW_COMMITS = 0;
const char *BRANCH          = "";
const char *COMMIT_HASH     = "";
const char *SHORT_VERSION   = "0.7.0";
const char *FULL_VERSION    = "";
const char *BUILD_DATE      = "";
}  // namespace version
}  // namespace prometheus
}  // namespace exporter
OPENTELEMETRY_END_NAMESPACE
#endif
