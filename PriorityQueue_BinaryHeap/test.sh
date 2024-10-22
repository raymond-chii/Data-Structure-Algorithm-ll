#!/bin/bash

# Function to compile the program
compile_program() {
    echo "Compiling the program..."
    make clean
    make
    if [ $? -ne 0 ]; then
        echo "Compilation failed!"
        exit 1
    fi
    echo "Compilation successful!"
    echo
}

# Function to run test with input and check output
run_test() {
    local test_name=$1
    local input=$2
    local expected_output=$3
    
    echo "Running test: $test_name"
    echo "Input:"
    echo "$input"
    echo
    
    # Run the test and capture output
    output=$(echo "$input" | ./useHeap.exe 2>&1)
    
    # Check if the output contains the expected string
    if echo "$output" | grep -q "$expected_output"; then
        echo "✅ Test passed!"
    else
        echo "❌ Test failed!"
        echo "Expected to find: $expected_output"
        echo "Actual output:"
        echo "$output"
    fi
    echo "----------------------------------------"
}

# Clean and compile
compile_program

# Test 1: Basic insertion and deletion
run_test "Basic Insert and DeleteMin" \
"100
1
test1
10
1
test2
20
4
5" \
"Deleted item with string id \"test1\" and key 10"

# Test 2: Duplicate insertion
run_test "Duplicate Insert" \
"100
1
duplicate
10
1
duplicate
20
5" \
"Call to 'insert' returned: 2"

# Test 3: Set key operation
run_test "Set Key" \
"100
1
test3
30
2
test3
15
4
5" \
"Deleted item with string id \"test3\" and key 15"

# Test 4: Remove operation
run_test "Remove Operation" \
"100
1
remove_test
25
3
remove_test
5" \
"Deleted item with string id \"remove_test\" and key 25"

# Test 5: Empty heap operations
run_test "Empty Heap" \
"100
4
5" \
"Call to 'deleteMin' returned: 1"

# Test 6: Complex sequence
run_test "Complex Operations" \
"100
1
item1
50
1
item2
30
1
item3
40
2
item2
20
4
1
item4
35
3
item3
4
5" \
"Deleted item with string id \"item2\" and key 20"

# Test 7: Capacity limit
run_test "Capacity Limit" \
"2
1
full1
10
1
full2
20
1
full3
30
5" \
"Call to 'insert' returned: 1"

# Test 8: Remove non-existent item
run_test "Remove Non-existent" \
"100
3
nonexistent
5" \
"Call to 'delete' returned: 1"

# Test 9: Set key for non-existent item
run_test "Set Key Non-existent" \
"100
2
nonexistent
50
5" \
"Call to 'setKey' returned: 1"

# Test 10: Mixed operations sequence
run_test "Mixed Operations" \
"100
1
mix1
45
1
mix2
35
1
mix3
25
4
2
mix2
15
3
mix3
4
5" \
"Deleted item with string id \"mix3\" and key 25"

echo "All tests completed!"


make clean
