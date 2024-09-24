def clean_dictionary(input_file, output_file):
    with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
        for line in infile:
            word = line.split(',')[0].strip()
            if word:
                outfile.write(word + '\n')

    print(f"Cleaned dictionary saved to {output_file}")

input_file = 'ngram_freq.csv'  
output_file = 'cleaned_dictionary.txt'
clean_dictionary(input_file, output_file)