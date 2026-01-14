#include <iostream>
#include <memory>

#if defined(__linux__) || defined(__linux) || defined(linux) ||                \
    defined(__gnu_linux__)
#define ON_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
#define ON_MACOS
#elif defined(_WIN32) || defined(_WIN64)
#define ON_WINDOWS
#endif

#if defined(ON_LINUX)

// HINT: allocate huge pages using mmap/munmap
// NOTE: See HugePagesSetupTips.md for how to enable huge pages in the OS
#include <sys/mman.h>

#elif defined(ON_WINDOWS)

// HINT: allocate huge pages using VirtualAlloc/VirtualFree
// NOTE: See HugePagesSetupTips.md for how to enable huge pages in the OS.
// The following boilerplate has been included to save you some time. You'll
// need to call `setRequiredPrivileges()` once per program run to tell the OS
// that you want to use huge pages. Additionally, you'll need to set the
// relevant user account privilege. The following code should do this for you,
// provided you run it as admin (run as admin -> reboot -> you're set to run as
// user).
// HINT: A good way to execute a function once per program run is to call it in
// the initialization of a function static variable.

#define UNICODE
#define _UNICODE

#include <windows.h>

#include <Sddl.h>
#include <ntsecapi.h>
#include <ntstatus.h>

// Based on
// https://stackoverflow.com/questions/42354504/enable-large-pages-in-windows-programmatically
namespace detail {
inline void InitLsaString(PLSA_UNICODE_STRING LsaString, LPWSTR String) {
  DWORD StringLength;

  if (String == NULL) {
    LsaString->Buffer = NULL;
    LsaString->Length = 0;
    LsaString->MaximumLength = 0;
    return;
  }

  StringLength = wcslen(String);
  LsaString->Buffer = String;
  LsaString->Length = (USHORT)StringLength * sizeof(WCHAR);
  LsaString->MaximumLength = (USHORT)(StringLength + 1) * sizeof(WCHAR);
}

inline auto openProcToken(DWORD desired_access) {
  using handle_t = std::pointer_traits<HANDLE>::element_type;
  constexpr auto handle_cleanup = [](HANDLE ptr) { CloseHandle(ptr); };
  using ret_t = std::unique_ptr<handle_t, decltype(+handle_cleanup)>;

  HANDLE handle{};
  if (!OpenProcessToken(GetCurrentProcess(), desired_access, &handle))
    throw std::runtime_error{"OpenProcessToken failed"};
  return ret_t{handle, +handle_cleanup};
}

inline auto getUserToken() {
  auto proc_token = detail::openProcToken(TOKEN_QUERY);

  // Probe the buffer size reqired for PTOKEN_USER structure
  DWORD dwbuf_sz = 0;
  if (!GetTokenInformation(proc_token.get(), TokenUser, nullptr, 0,
                              &dwbuf_sz) &&
      (GetLastError() != ERROR_INSUFFICIENT_BUFFER))
    throw std::runtime_error{"GetTokenInformation failed"};

  // Retrieve the token information in a TOKEN_USER structure
  constexpr auto deleter = [](PTOKEN_USER ptr) { free(ptr); };
  PTOKEN_USER ptr = (PTOKEN_USER)malloc(dwbuf_sz);
  std::unique_ptr<TOKEN_USER, decltype(deleter)> user_token{ptr, deleter};
  if (!GetTokenInformation(proc_token.get(), TokenUser, user_token.get(),
                              dwbuf_sz, &dwbuf_sz))
    throw std::runtime_error{"GetTokenInformation failed"};

  return user_token;
}

inline void adjustAccountPrivilege() {
  auto user_token = getUserToken();

  LSA_OBJECT_ATTRIBUTES obj_attrib{};
  LSA_HANDLE policy_handle;
  if (LsaOpenPolicy(nullptr, &obj_attrib,
                    POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES,
                    &policy_handle))
    throw std::runtime_error{"LsaOpenPolicy failed"};

  LSA_UNICODE_STRING privilege_string;
  InitLsaString(&privilege_string, SE_LOCK_MEMORY_NAME);
  if (LsaAddAccountRights(policy_handle, user_token->User.Sid,
                          &privilege_string, 1))
    throw std::runtime_error{"LsaAddAccountRights failed"};
}

inline bool enableProcPrivilege() {
  auto proc_token = openProcToken(TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES);

  TOKEN_PRIVILEGES priv_token{};
  priv_token.PrivilegeCount = 1;
  priv_token.Privileges->Attributes = SE_PRIVILEGE_ENABLED;

  if (!LookupPrivilegeValue(NULL, SE_LOCK_MEMORY_NAME,
                               &priv_token.Privileges->Luid))
    throw std::runtime_error{"LookupPrivilegeValue failed"};

  if (!AdjustTokenPrivileges(proc_token.get(), FALSE, &priv_token, 0,
                                nullptr, 0))
    throw std::runtime_error{"AdjustTokenPrivileges failed"};

  if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    return false;
  else
    return true;
}
} // namespace detail

inline bool setRequiredPrivileges() {
  if (detail::enableProcPrivilege())
    return true;
  else {
    std::clog << "It seems that your user account does not have the privilege "
                 "required to allocate huge pages. The program will now "
                 "attempt to give you this privilege. This requires the "
                 "program to be run as admin."
              << std::endl;
    detail::adjustAccountPrivilege();
    std::clog << "The required privilege has been set successfully. Please log "
                 "out and log back in for the changes to take effect. This "
                 "program will now terminate."
              << std::endl;
    std::terminate();
    return false;
  }
}

static bool init = setRequiredPrivileges();

#endif
#define SOLUTION
// Allocate an array of doubles of size `size`, return it as a
// std::unique_ptr<double[], D>, where `D` is a custom deleter type
inline auto allocateDoublesArray(size_t size) {
  // Allocate memory
#ifndef SOLUTION
  double* alloc = new double[size];
  // remember to cast the pointer to double* if your allocator returns void*

  // Deleters can be conveniently defined as lambdas, but you can explicitly
  // define a class if you're not comfortable with the syntax
  auto deleter = [/* state = ... */](double* ptr) { delete[] ptr; };

  return std::unique_ptr<double[], decltype(deleter)>(alloc,
                                                      std::move(deleter));
#else

#if defined(ON_LINUX)
#define MAP_HUGE_2MB (21 << MAP_HUGE_SHIFT)
#define MAP_HUGE_1GB (30 << MAP_HUGE_SHIFT)
  double* buf =
      (double*)mmap(NULL, size * sizeof(double), PROT_WRITE | PROT_READ,
                    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
  if (buf == MAP_FAILED) {
    perror("mmaped");
    std::exit(EXIT_FAILURE);
  }

  auto deleter = [size](double* ptr) { munmap(ptr, size * sizeof(double)); };

  buf[0] = 1;

  return std::unique_ptr<double[], decltype(deleter)>((double*)buf,
                                                      std::move(deleter));

#elif defined(ON_WINDOWS)


  SIZE_T allocSize = sizeof(double) * size;

 allocSize =
      ((allocSize + 2 * 1024 * 1024) / (2 * 1024 * 1024)) * 2 * 1024 * 1024;

  LPVOID lpMemBase =
      VirtualAlloc(NULL, allocSize, MEM_COMMIT | MEM_RESERVE | MEM_LARGE_PAGES,
                   PAGE_READWRITE);

  if (lpMemBase) {
    printf("can not allocate\n");
  }

  double* alloc = (double*)lpMemBase;
  // remember to cast the pointer to double* if your allocator returns void*

  // Deleters can be conveniently defined as lambdas, but you can explicitly
  // define a class if you're not comfortable with the syntax
  auto deleter = [size, lpMemBase](double* ptr) {
    VirtualFree(lpMemBase, 0, MEM_RELEASE);
  };

  return std::unique_ptr<double[], decltype(deleter)>(alloc,
                                                      std::move(deleter));
#else
  double* alloc = new double[size];
  // remember to cast the pointer to double* if your allocator returns void*

  // Deleters can be conveniently defined as lambdas, but you can explicitly
  // define a class if you're not comfortable with the syntax
  auto deleter = [/* state = ... */](double* ptr) { delete[] ptr; };

  return std::unique_ptr<double[], decltype(deleter)>(alloc,
                                                      std::move(deleter));

#endif

#endif
  // The above is equivalent to:
  // return std::make_unique<double[]>(size);
  // The more verbose version is meant to demonstrate the use of a custom
  // (potentially stateful) deleter
}
