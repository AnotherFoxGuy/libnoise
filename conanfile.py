from conans import ConanFile, CMake, tools


class LibnoiseConan(ConanFile):
    name = "libnoise"
    version = "1.0.0"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Libnoise here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "cmake*", "src*", "noiseutils*", "CMakeLists.txt"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.definitions['BUILD_SHARED_LIBS'] = 'OFF'
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.includedirs = ['include',
                                     'include/noise',
                                     'include/noise/module',
                                     'include/noise/model',
                                     ]
        self.cpp_info.libs = tools.collect_libs(self)
