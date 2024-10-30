# executable and name of the benchmark needs to be appended to the file before use!

./atax_base_dump_arrays 2> test_output.txt && diff reference_output.txt test_output.txt
