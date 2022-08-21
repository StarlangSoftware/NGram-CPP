from conans import ConanFile, CMake, tools


class NGramConan(ConanFile):
    name = "NGram"
    version = "1.0.0"
    requires = ["Math/1.0.0", "Sampling/1.0.0", "DataStructure/1.0.0"]
    license = "GPL License"
    author = "Olcay Taner Yildiz olcay.yildiz@ozyegin.edu.tr"
    url = "https://github.com/StarlangSoftware/NGram-CPP"
    description = "NGram library"
    topics = ("")
    generators = "cmake"
    exports_sources = "src/*"
    no_copy_source = True

    def package(self):
        self.copy("*.h", dst="include", keep_path=False)
