#ifndef domTest_h
#define domTest_h

#include <map>
#include <string>

// We use the boost filesystem library for cross-platform file system support. You'll need
// to have boost on your machine for this to work. For the Windows build boost is provided
// in the external-libs folder, but for Linux/Mac it's expected that you'll install a boost
// obtained via your distro's package manager. For example on Debian/Ubuntu, you can run
//   apt-get install libboost-filesystem-dev
// to install the boost filesystem library on your machine.
#include <boost/filesystem/convenience.hpp>

struct domTest;
std::map<std::string, domTest*>& registeredTests();
boost::filesystem::path& dataPath();
boost::filesystem::path& tmpPath();

struct testResult {
	bool passed;
	int line; // If the test failed, the line number it failed on, or -1 if the line
	          // number isn't available.
	std::string msg; // An error message for the user. Usually empty.

	testResult() : passed(true), line(-1) { }
	testResult(bool passed, int line = -1, const std::string& msg = "")
	  : passed(passed),
	    line(line),
	    msg(msg) {
	}
};

struct domTest {
	std::string name;
	domTest(const std::string& name) : name(name) {
		registeredTests()[name] = this;
	}
	virtual ~domTest() { };
	virtual testResult run() = 0;
};

#define DefineTest(testName) \
	struct domTest_##testName : public domTest { \
		domTest_##testName() : domTest(#testName) { } \
		testResult run();	\
	}; \
	domTest_##testName domTest_##testName##Obj; \
	testResult domTest_##testName::run()

#define CheckResult(val) \
	if (!(val)) { \
		return testResult(false, __LINE__); \
	}

std::string lookupTestFile(const std::string& fileName);

#endif
