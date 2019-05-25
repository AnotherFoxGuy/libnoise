from conans import ConanFile, CMake, tools


class LibnoiseConan(ConanFile):
    name = "libnoise"
    version = "1.0.1"
    license = "GLP 2.1"
    url = "https://github.com/AnotherFoxGuy/libnoise"
    description = "libnoise is a portable C++ library that is used to generate coherent noise, a type of smoothly-changing noise. libnoise can generate Perlin noise, ridged multifractal noise, and other types of coherent-noise."
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "cmake*", "src*", "include*", "noiseutils*", "CMakeLists.txt"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.definitions['BUILD_SHARED_LIBS'] = 'OFF'
        #cmake.definitions['BUILD_LIBNOISE_UTILS'] = 'OFF'
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
        self.cpp_info.defines = ['NOISE_STATIC']
