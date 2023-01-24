# a list of multiple python versions to build
$PYTHON_VERSIONS = @("3.7", "3.8", "3.9", "3.10")
foreach ($PYTHON_VERSION in $PYTHON_VERSIONS) {
    docker run --rm `
        -v ${PWD}:/work `
        gcszhn/miniconda3-manylinux2014_x64:latest `
        bash build-manylinux2014-x64.sh $PYTHON_VERSION
}
Push-Location dist
Get-ChildItem | ForEach-Object { 
    $new_name = $_ -replace "-linux", "-manylinux2014"
    if ($new_name -eq $_) {
        return
    }
    Write-Output "Rename $_ to $new_name"
    Rename-Item $_ $new_name
}
Pop-Location

