from skbuild import setup
import versioneer

setup(
    packages=["diamond4py"],
    version=versioneer.get_version(),
    cmdclass=versioneer.get_cmdclass()
)
