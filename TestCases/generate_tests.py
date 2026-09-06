import random
import string
import os

def count_occurrences(text, query):
    """
    Counts overlapping occurrences of query in text.
    """
    if not query:
        return 0
    count = 0
    start = 0
    while True:
        idx = text.find(query, start)
        if idx == -1:
            break
        count += 1
        start = idx + 1
    return count

def write_test_case(f_in, f_out, text, queries):
    """
    Writes a test case to input file and corresponding expected output.
    """
    f_in.write("TEXT\n")
    f_in.write(text + "\n")
    
    if isinstance(queries, str):
        queries = [queries]
        
    for q in queries:
        f_in.write("QUERY\n")
        f_in.write(q + "\n")
        
        # Calculate expected output
        expected_count = count_occurrences(text, q)
        f_out.write(f"{expected_count}\n")
    
    f_in.write("\n")

def generate_queries(text, alphabet, count_exist=50, count_non_exist=10):
    """
    Generates a list of random queries.
    count_exist: number of queries sampled from the text (should exist)
    count_non_exist: number of random queries (likely won't exist)
    """
    queries = []
    text_len = len(text)
    
    # Generate queries that exist by sampling substrings
    if text_len > 0:
        for _ in range(count_exist):
            # Pick a random length between 5 and 50
            q_len = random.randint(5, 50)
            if q_len > text_len:
                q_len = text_len
            start = random.randint(0, text_len - q_len)
            queries.append(text[start:start+q_len])
            
    # Generate queries that likely don't exist
    for _ in range(count_non_exist):
        q_len = random.randint(5, 50)
        queries.append("".join(random.choices(alphabet, k=q_len)))
        
    return queries

def generate_tests_random():
    input_filepath = "TestCases/Input/TestsRandom.txt"
    output_filepath = "TestCases/ExpectedOutput/TestsRandom.txt"
    
    with open(input_filepath, "w") as f_in, open(output_filepath, "w") as f_out:
        alphabet = "abcdefghijklmnopqrstuvwxyz. $'@#?/,;:-_"
        
        # 1. Random text length 1,000 with 10 queries
        text_1k = "".join(random.choices(alphabet, k=1000))
        queries_1k = []
        # Random queries that exist
        for _ in range(5):
            start = random.randint(0, 990)
            length = random.randint(1, 10)
            queries_1k.append(text_1k[start:start+length])
        # Random queries that likely don't exist
        for _ in range(5):
             queries_1k.append("".join(random.choices(alphabet, k=10)))
        
        write_test_case(f_in, f_out, text_1k, queries_1k)

        # 2. Random text length 10,000 with 20 queries
        text_10k = "".join(random.choices(alphabet, k=10000))
        queries_10k = []
        for _ in range(10):
            start = random.randint(0, 9900)
            length = random.randint(3, 15)
            queries_10k.append(text_10k[start:start+length])
        for _ in range(10):
            queries_10k.append("".join(random.choices(alphabet, k=15)))
            
        write_test_case(f_in, f_out, text_10k, queries_10k)

def generate_35mb():
    input_filepath = "TestCases/Input/35MB.txt"
    output_filepath = "TestCases/ExpectedOutput/35MB.txt"

    target_size = 35 * 1024 * 1024
    alphabet = "abcdefghijklmnopqrstuvwxyz. $'@#?/,;:-_"
    
    chunk_size = 1024 * 1024 
    with open(input_filepath, "w") as f_in, open(output_filepath, "w") as f_out:
        f_in.write("TEXT\n")
        
        full_text_list = []
        bytes_written = 0
        while bytes_written < target_size:
            chunk = "".join(random.choices(alphabet, k=chunk_size))
            full_text_list.append(chunk)
            bytes_written += len(chunk)
            f_in.write(chunk)
            
        full_text = "".join(full_text_list)
    
        queries = generate_queries(full_text, alphabet, count_exist=50, count_non_exist=10)
        
        if full_text:
             queries.append(full_text[-10:])
             queries.append(full_text[-50:])
        queries.append("xyz_nonexistent")
        
        for q in queries:
            f_in.write("\nQUERY\n")
            f_in.write(q + "\n")
      
            expected_count = count_occurrences(full_text, q)
            f_out.write(f"{expected_count}\n")
            
        f_in.write("\n")

def generate_10mb():
    input_filepath = "TestCases/Input/10MB.txt"
    output_filepath = "TestCases/ExpectedOutput/10MB.txt"
    
    # 10MB file
    target_size = 10 * 1024 * 1024
    alphabet = "abcdefghijklmnopqrstuvwxyz. $'@#?/,;:-_"
    
    chunk_size = 1024 * 1024 
    with open(input_filepath, "w") as f_in, open(output_filepath, "w") as f_out:
        f_in.write("TEXT\n")
        
        full_text_list = []
        bytes_written = 0
        while bytes_written < target_size:
            chunk = "".join(random.choices(alphabet, k=chunk_size))
            full_text_list.append(chunk)
            bytes_written += len(chunk)
            f_in.write(chunk)
            
        full_text = "".join(full_text_list)
        
        queries = generate_queries(full_text, alphabet, count_exist=50, count_non_exist=10)
        
        # Add specific queries
        if full_text:
             queries.append(full_text[-10:])
             queries.append(full_text[-50:])
        queries.append("xyz_notexisting")
        queries.append("abc_hello world?")
        
        for q in queries:
            f_in.write("\nQUERY\n")
            f_in.write(q + "\n")
            
            expected_count = count_occurrences(full_text, q)
            f_out.write(f"{expected_count}\n")
            
        f_in.write("\n")

if __name__ == "__main__":
    generate_tests_random()
    generate_10mb()
    generate_35mb()