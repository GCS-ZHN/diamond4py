# find install location of scikit-build
pip show scikit-build | Select-String -Pattern "Location" | ForEach-Object {
    $_ = $_ -replace "Location: ", ""
    $CMAKE_MODULE_PATH = Join-Path -Path $_ -ChildPath "skbuild/resources/cmake"
}
if (-not $CMAKE_MODULE_PATH) {
    Write-Error "Could not find scikit-build install location"
    exit 1
}

#Check if zlib is installed
conda list | Select-String -Pattern "zlib"
if ($LastExitCode -ne 0) {
    Write-Error "zlib is not installed"
    exit 1
} else {
    $ZLIB_INCLUDE_DIR = Join-Path -Path $env:CONDA_PREFIX -ChildPath "Library/include"
    $ZLIB_LIBRARY = Join-Path -Path $env:CONDA_PREFIX -ChildPath "Library/lib/zlib.lib"
}


# remove build if exists
$buildPath = Join-Path -Path $PSScriptRoot -ChildPath "build"
if (Test-Path -Path $buildPath) {
    Remove-Item -Path $buildPath -Recurse -Force
    if (Test-Path -Path $buildPath) {
        Write-Error "Could not remove build folder"
        exit 1
    }
}

# create build folder
New-Item -Path $buildPath -ItemType Directory | Out-Null

# replace \ with / for cmake
$CMAKE_MODULE_PATH = $CMAKE_MODULE_PATH -replace "\\","/"
$CMAKE_MODULE_PATH = $CMAKE_MODULE_PATH -replace "c:/","C:/"
$ZLIB_INCLUDE_DIR = $ZLIB_INCLUDE_DIR -replace "\\","/"
$ZLIB_LIBRARY = $ZLIB_LIBRARY -replace "\\","/"
$CMAKE_INSTALL_PREFIX = $PSScriptRoot -replace "\\","/"
Write-OutPut $CMAKE_MODULE_PATH
Write-OutPut $ZLIB_INCLUDE_DIR
Write-OutPut $ZLIB_LIBRARY
Write-OutPut $CMAKE_INSTALL_PREFIX

# enter build folder and run cmake
$BUILD_TYPE = "Release"
Push-Location -Path $buildPath
cmake .. "-DCMAKE_MODULE_PATH=$CMAKE_MODULE_PATH" `
    "--install-prefix=$CMAKE_INSTALL_PREFIX" `
    "-DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR" `
    "-DZLIB_LIBRARY=$ZLIB_LIBRARY" `
    "-DCMAKE_BUILD_TYPE=$BUILD_TYPE"

if ($LastExitCode -ne 0) {
    Write-Error "CMake failed"
    Pop-Location
    exit 1
}

# run build and install
cmake --build . --target install ALL_BUILD  --config $BUILD_TYPE
if ($LastExitCode -ne 0) {
    Write-Error "Build failed"
    Pop-Location
    exit 1
}



# exit build folder must execute before exit
Pop-Location