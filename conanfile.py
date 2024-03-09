from os.path import join

from conan import ConanFile
from conan.tools.files import copy


class NGramConan(ConanFile):
    name = "n_gram"
    version = "1.0.0"
    requires = ["math/1.0.0", "sampling/1.0.0", "data_structure/1.0.0"]
    license = "GPL License"
    author = "Olcay Taner Yildiz olcay.yildiz@ozyegin.edu.tr"
    url = "https://github.com/StarlangSoftware/NGram-CPP"
    description = "NGram library"
    topics = ("")
    exports_sources = "src/*"
    no_copy_source = True

    def package(self):
        copy(conanfile=self, keep_path=False, src=join(self.source_folder), dst=join(self.package_folder, "include"), pattern="*.h")
