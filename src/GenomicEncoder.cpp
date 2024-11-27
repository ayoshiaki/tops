#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

/* class that receives a sequence and a coding map to encode the sequence */
class GenomicSequenceEncoder {
public:
    GenomicSequenceEncoder(const std::string& sequence) : sequence(sequence) {
        // Check if the sequence contains valid nucleotides
        for (char nucleotide : sequence) {
            if (nucleotide != 'A' && nucleotide != 'C' && nucleotide != 'G' && nucleotide != 'T') {
                throw std::invalid_argument("Sequence contains invalid nucleotide.");
            }
        }
    }

    /*
    Method to create a matrix where the number of columns is the length of the sequence divided by the k-mer size (k), 
    and the number of rows is the size of the encoding map. 
    Each cell in this matrix will be 0 except for the position corresponding to the k-mer, which will be 1.
    */
    std::vector<std::vector<int>> doEncoding(const std::unordered_map<std::string, int>& encodingMap, int k) {
        int numKmers = sequence.size() / k;
        int numRows = encodingMap.size();
        
        // Initialize the matrix with zeros
        std::vector<std::vector<int>> encodingMatrix(numRows, std::vector<int>(numKmers, 0));

        for (size_t i = 0; i < numKmers; ++i) {
            std::string kmer = sequence.substr(i * k, k);
            if (encodingMap.find(kmer) != encodingMap.end()) {
                int rowIndex = encodingMap.at(kmer);
                encodingMatrix[rowIndex][i] = 1;
            } else {
                throw std::invalid_argument("Sequence contains invalid k-mer for the provided encoding map.");
            }
        }

        return encodingMatrix;
    }

    void show_encoded(std::vector<std::vector<int>> encoded_sequence){
        std::cout << "k-mer encoding:\n";
        for (const auto& row : encoded_sequence) {
            for (int val : row) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
    }

    /*
    some basic pre-defined encoding methods
    */

    // one-hot encoding
    std::vector<std::vector<int>> oneHotEncoding() {
        return doEncoding(oneHotMap, 1);
    }

    // dinucleotide encoding
    std::vector<std::vector<int>> dinucleotideEncoding() {
        return doEncoding(dinucleotideMap, 2);
    }

    // codon encoding
    std::vector<std::vector<int>> codonEncoding() {
        return doEncoding(codonMap, 3);
    }

private:
    std::string sequence;

    // One-hot encoding
    std::unordered_map<std::string, int> oneHotMap = {
        {"A", 0},
        {"C", 1},
        {"G", 2},
        {"T", 3}
    };

    // Dinucleotide encoding
    std::unordered_map<std::string, int> dinucleotideMap = {
        {"AA", 0}, {"AC", 1}, {"AG", 2}, {"AT", 3},
        {"CA", 4}, {"CC", 5}, {"CG", 6}, {"CT", 7},
        {"GA", 8}, {"GC", 9}, {"GG", 10}, {"GT", 11},
        {"TA", 12}, {"TC", 13}, {"TG", 14}, {"TT", 15}
    };

    // Codon encoding
    std::unordered_map<std::string, int> codonMap = {
        {"AAA", 0}, {"AAC", 1}, {"AAG", 2}, {"AAT", 3},
        {"ACA", 4}, {"ACC", 5}, {"ACG", 6}, {"ACT", 7},
        {"AGA", 8}, {"AGC", 9}, {"AGG", 10}, {"AGT", 11},
        {"ATA", 12}, {"ATC", 13}, {"ATG", 14}, {"ATT", 15},
        {"CAA", 16}, {"CAC", 17}, {"CAG", 18}, {"CAT", 19},
        {"CCA", 20}, {"CCC", 21}, {"CCG", 22}, {"CCT", 23},
        {"CGA", 24}, {"CGC", 25}, {"CGG", 26}, {"CGT", 27},
        {"CTA", 28}, {"CTC", 29}, {"CTG", 30}, {"CTT", 31},
        {"GAA", 32}, {"GAC", 33}, {"GAG", 34}, {"GAT", 35},
        {"GCA", 36}, {"GCC", 37}, {"GCG", 38}, {"GCT", 39},
        {"GGA", 40}, {"GGC", 41}, {"GGG", 42}, {"GGT", 43},
        {"GTA", 44}, {"GTC", 45}, {"GTG", 46}, {"GTT", 47},
        {"TAA", 48}, {"TAC", 49}, {"TAG", 50}, {"TAT", 51},
        {"TCA", 52}, {"TCC", 53}, {"TCG", 54}, {"TCT", 55},
        {"TGA", 56}, {"TGC", 57}, {"TGG", 58}, {"TGT", 59},
        {"TTA", 60}, {"TTC", 61}, {"TTG", 62}, {"TTT", 63}
    };
};

// void test() {
//     std::string sequence = "ACGTACGTGACT";

//     GenomicSequenceEncoder encoder(sequence);

//     auto oneHot = encoder.oneHotEncoding();
//     encoder.show_encoded(oneHot);
    
//     auto dinucleotide = encoder.dinucleotideEncoding();
//     encoder.show_encoded(dinucleotide);

//     auto codon = encoder.codonEncoding();
//     encoder.show_encoded(codon);
// }
