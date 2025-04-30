#include "utility.h"

NAMESPACE_BEGIN
class abc;
namespace Neptune { namespace Timing { class Timer; } }

//FWD_DECL_END

namespace Neptune {
    namespace Utilities {
        class FileSystem {
        public:
            static std::string GetCurrentDirectory();
            static std::string GetFileName(const std::string& path);
            static std::string GetFileExtension(const std::string& path);
            static std::string GetFileNameWithoutExtension(const std::string& path);
            static bool FileExists(const std::string& path);
            static bool DirectoryExists(const std::string& path);
            static void CreateDirectory(const std::string& path);
        };
    }
    namespace Timing {
        class Timer;
        class TimingParams;
        class TmDelay;
        class TmEdge;
        class TmConstraint;
        class TmRecord;
        namespace TmType {
            enum TmClockSense{};
        };
    }
    class PackingMgr;
}


NAMESPACE_END
