make clean
make release
make run-release
python3 printlayout.py ./input/sample.in output_release.out
./Solutionchecker/Solutionchecker ./input/sample.in output_release.out