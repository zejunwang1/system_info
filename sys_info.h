#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <cstring>
#include <string>
#include <sstream>
#include <fstream>

#ifdef __linux__
  #include <arpa/inet.h>
  #include <sys/utsname.h>
  #include <unistd.h>
#elif _WIN32
  /* If defined, the following flags inhibit definition
   * of the indicated items.
   */
  #define NOGDICAPMASKS   // - CC_*, LC_*, PC_*, CP_*, TC_*, RC_
  #define NOVIRTUALKEYCODES // VK_*
  #define NOWINMESSAGES   // WM_*, EM_*, LB_*, CB_*
  #define NOWINSTYLES     // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
  #define NOSYSMETRICS    // SM_*
  #define NOMENUS       // MF_*
  #define NOICONS       // IDI_*
  #define NOKEYSTATES     // MK_*
  #define NOSYSCOMMANDS   // SC_*
  #define NORASTEROPS     // Binary and Tertiary raster ops
  #define NOSHOWWINDOW    // SW_*
  #define OEMRESOURCE     // OEM Resource values
  #define NOATOM        // Atom Manager routines
  #define NOCLIPBOARD     // Clipboard routines
  #define NOCOLOR       // Screen colors
  #define NOCTLMGR      // Control and Dialog routines
  #define NODRAWTEXT      // DrawText() and DT_*
  #define NOGDI       // All GDI defines and routines
  #define NOKERNEL      // All KERNEL defines and routines
  #define NOUSER        // All USER defines and routines
  #define NONLS       // All NLS defines and routines
  #define NOMB        // MB_* and MessageBox()
  #define NOMEMMGR      // GMEM_*, LMEM_*, GHND, LHND, associated routines
  #define NOMETAFILE      // typedef METAFILEPICT
  #define NOMINMAX      // Macros min(a,b) and max(a,b)
  #define NOMSG       // typedef MSG and associated routines
  #define NOOPENFILE      // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
  #define NOSCROLL      // SB_* and scrolling routines
  #define NOSERVICE     // All Service Controller routines, SERVICE_ equates, etc.
  #define NOSOUND       // Sound driver routines
  #define NOTEXTMETRIC    // typedef TEXTMETRIC and associated routines
  #define NOWH        // SetWindowsHook and WH_*
  #define NOWINOFFSETS    // GWL_*, GCL_*, associated routines
  #define NOCOMM        // COMM driver routines
  #define NOKANJI             // Kanji support stuff.
  #define NOHELP              // Help engine interface.
  #define NOPROFILER      // Profiler interface.
  #define NODEFERWINDOWPOS  // DeferWindowPos routines
  #define NOMCX       // Modem Configuration Extension
  #include <windows.h>
#elif _OSX
  // some OSX header
#endif 
    

class SystemInfo {
  public:
    SystemInfo() {}
    
    // total RAM in MiB
    std::uint64_t totalRAM() {
      #ifdef __linux__
        auto numPages = sysconf(_SC_PHYS_PAGES);
        auto pageSize = sysconf(_SC_PAGE_SIZE);
      
        // convert to MiB from B
        return numPages * pageSize / (1024 * 1024);
      #elif _WIN32
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);

        // convert to MiB from B
        return statex.ullTotalPhys / (1024 * 1024);
      #elif _OSX
      #endif       
    }
    
    std::string CPUModel() {
      int model[12];
      #ifdef __linux__
        auto callId = 0x80000002;
        asm volatile ("cpuid" : "=a"(model[0]), "=b"(model[1]), "=c"(model[2]), "=d"(model[3]) : "a"(callId));
        ++callId;
        asm volatile ("cpuid" : "=a"(model[4]), "=b"(model[5]), "=c"(model[6]), "=d"(model[7]) : "a"(callId));
        ++callId;
        asm volatile ("cpuid" : "=a"(model[8]), "=b"(model[9]), "=c"(model[10]), "=d"(model[11]) : "a"(callId));
      #elif _WIN32
        __cpuidex(model + 0, 0x80000002, 0);
        __cpuidex(model + 4, 0x80000003, 0);
        __cpuidex(model + 8, 0x80000004, 0);
      #elif _OSX
      #endif
      
      // model is null-terminated
      std::string modelStr{reinterpret_cast<const char*>(model)};

      // remove leading spaces (string is provided right-justified)
      modelStr.erase(0, modelStr.find_first_not_of(' '));
      return modelStr;
    }
    
    std::string OSName() {
      #ifdef __linux__
        utsname linuxInfo;
        if(uname(&linuxInfo) == -1)
          return "";
        return static_cast<std::string>(linuxInfo.sysname);
      #elif _WIN32
        return "Windows";
      #elif _OSX
        return "OSX";
      #endif
    }
    
    std::string OSVersion() {
      #ifdef __linux__
        utsname linuxInfo;
        if(uname(&linuxInfo) == -1)
          return "";
        return static_cast<std::string>(linuxInfo.release);
      #elif _WIN32
        OSVERSIONINFOEX osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osvi));

        std::ostringstream oss;
        oss << osvi.dwMajorVersion << '.' << osvi.dwMinorVersion << '.' + osvi.dwBuildNumber;
        oss << " SP" << osvi.wServicePackMajor << '.' << osvi.wServicePackMinor;
        return oss.str();
      #elif _OSX
      #endif
    }
    
    std::string OSArchitecture() {
      #ifdef __linux__
        utsname info;
        if(uname(&info) == -1)
          return "";
        return static_cast<std::string>(info.machine);
      #elif _WIN32
        SYSTEM_INFO sysInfo;
        GetNativeSystemInfo(&sysInfo);
        return std::to_string(sysInfo.wProcessorArchitecture);
      #elif _OSX
      #endif
    }
  
  #ifdef __linux__
  std::string CPUID() {
    std::string cpu_id;
    unsigned int s1 = 0;
    unsigned int s2 = 0;
    asm volatile (
      "movl $0x01, %%eax; \n\t"
      "xorl %%edx, %%edx; \n\t"
      "cpuid; \n\t"
      "movl %%edx, %0; \n\t"
      "movl %%eax, %1; \n\t"
      : "=m" (s1), "=m" (s2));
    
    if ((0 == s1) && (0 == s2))
      return cpu_id;

    char cpu[128];
    memset(cpu, 0, sizeof(cpu));
    snprintf(cpu, sizeof(cpu), "%08X%08X", htonl(s2), htonl(s1));
    cpu_id.assign(cpu);
    return cpu_id;
  }
  
  void parseDiskSerial(const char *file_name, const char *match_words, std::string &serial_no) {
    std::ifstream ifs(file_name, std::ios::binary);
    if (!ifs.is_open())
      return;

    char line[4096] = { 0 };
    while (!ifs.eof()) {
      ifs.getline(line, sizeof(line));
      if (!ifs.good())
        break;
      const char *board = strstr(line, match_words);
      if (NULL == board)
        continue;

      board += strlen(match_words);
      while ('\0' != board[0]) {
        if (' ' != board[0])
          serial_no.push_back(board[0]);
        ++board;
      }

      if ("None" == serial_no) {
        serial_no.clear();
        continue;
      }
      if (!serial_no.empty())
        break;
    }
    ifs.close();
  }
  
  std::string DiskID() {
    std::string serial_no;
    std::string disk_name = "/dev/sda";
    const char *dmidecode_result = ".dmidecode_result.txt";
    char command[512] = { 0 };
    snprintf(command, sizeof(command), 
            "udevadm info --query=all --name=%s | grep ID_SERIAL_SHORT= > %s", 
            disk_name.c_str(), dmidecode_result);

    if (0 == system(command))
      parseDiskSerial(dmidecode_result, "ID_SERIAL_SHORT=", serial_no);

    unlink(dmidecode_result);
    return serial_no;
  }
  #endif
};

#endif
