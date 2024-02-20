#include "Sequence.hpp"
#include "Fasta.hpp"
#include <string>
#include <iostream> // pour cout/endl/...
#include <fstream>  // pour ifstream
#include <map>
#include <tuple>
#include <vector>

std::set<char> Fasta::legalMultiHeaderChar = {
    ';',
};

std::set<char> Fasta::legalMonoHeaderChar = {
    '>',
};

std::set<char> Fasta::legalCommentChar = {
    ';',
};

std::set<char> Fasta::blankChar = {
    (char)1,
    (char)2,
    (char)3,
    (char)4,
    (char)5,
    (char)6,
    (char)7,
    (char)8,
    (char)9,
    (char)10, // \n
    (char)11,
    (char)12,
    (char)13,
    (char)14,
    (char)15,
    (char)16,
    (char)17,
    (char)18,
    (char)19,
    (char)20,
    (char)21,
    (char)22,
    (char)23,
    (char)24,
    (char)25,
    (char)26,
    (char)27,
    (char)28,
    (char)29,
    (char)30,
    (char)31,
    (char)32,
};

bool Fasta::isHeaderChar(char header_char) {
    return (isMonoHeaderChar(header_char) || isMultiHeaderChar(header_char));
}

bool Fasta::isMonoHeaderChar(char header_char) {
    return (legalMonoHeaderChar.find(header_char) != legalMonoHeaderChar.end());
}

bool Fasta::isMultiHeaderChar(char header_char) {
    return (legalMultiHeaderChar.find(header_char) != legalMultiHeaderChar.end());
}

bool Fasta::isCommentChar(char comment_char) {
    return (legalCommentChar.find(comment_char) != legalCommentChar.end());
}

bool Fasta::isBlankChar(char blank) {
    return (blankChar.find(blank) != blankChar.end());
}



std::tuple<std::string, std::string, std::map<size_t, std::string>>
Fasta::parseFasta(std::ifstream& file_flux, unsigned int& current_line, bool verbose) {
    bool in_sequence = false;       // Do <current_char> is inside a sequence ?
    bool in_comment = false;        // Do <current_char> is inside a comment ?
    bool in_header = false;         // Do <current_char> is inside a header ?
    bool exit = false;              // Do the loop should stop now ?

    char current_char;      // Contain the current char that is read inside <file_flux>.
    unsigned current_column = 1;
    char last_char = '\n';  /* Contain the value that <current_char> contained during the last loop.
                             * This var is initialised at '\n' since we assume that <file_flux.get> will return the first char of a line. 
                             * With a few tweaks, this variable cane be removed. 
                             * I prefer to keep it, since it increase the readability of the code. */

    std::string header = "";    // Store chars that belong to the header.
    std::string sequence = "";  // Store chars that belong to the sequence.
    std::string current_comment = "";  // Store chars that belong to a comment.
    std::map<size_t, std::string> comments; // Store comments. Comments are stored using their positions inside the sequences.

    if (!file_flux.is_open()) {
        //TODO: raise error
        std::cout << "Can not use the file : "
                    << "'file_flux'" << std::endl;
        return std::tuple<std::string, std::string, std::map<size_t, std::string>> {
                        header,      sequence,    comments,         
        };
    }

    while (file_flux && !exit) {
        // --- Header & sequence verifications --- :
        if (last_char == '\n') {
            // → We are at the beginning of a new line
            
            current_char = file_flux.peek(); // We use <ifstream.peek> in order to avoid moving too far inside the file.
            if ((int)current_char == -1) {
                // Assure that the end of the file is treated as a line break
                current_char = '\n';
            }

            // --- --- --- Header Detection --- --- --- 
            if (isHeaderChar(current_char)) {    
                // → Header symbol at the beginning of the line.
        
                if (in_sequence) {
                    // A new header has been reached. This sequence end here.
                    exit = true;    // Will end the loop
                    continue;
                }

                if (in_header) {
                    // We are in a multi-line header. Nothing more to do.
                } else {
                    // We start the header of the sequence.
                    in_header = true;
                }

            // --- --- --- Sequence Detection --- --- --- 
            } else if (in_header) {
                // → We are leaving a header. (The last <if> assure that the next char does not announce a header)
                in_header = false;
                in_sequence = true;
            }
            current_column = 0;
            current_line += 1;

        }

        //  --- Advance in the file --- 
        current_char = (char)file_flux.get();
        current_column += 1;
        if ((int)current_char == -1) {
            // Assure that the end of the file is treated as a line break
            current_char = '\n';
        }

        // --- --- Header --- ---
        if (in_header){
            header += current_char;

        // --- --- Sequence --- ---
        } else if (in_sequence) {
            // Being inside a comment
            if (in_comment) {
                if (current_char == '\n') {
                    // We are leaving this comment
                    in_comment = false;

                    // Save the comment
                    if (comments.count(sequence.length()) >= 1){
                        comments[sequence.length()] += current_comment;

                    } else {
                        comments[sequence.length()] = current_comment;
                    }
                    
                    // clear <current_comment>
                    current_comment.clear();
        
                } else {
                    current_comment += current_char;
                }
            
            // Entering inside a comment
            } else if (isCommentChar(current_char)) {
                in_comment = true;
            
            // Normal sequence behaviour
            } else if (isBlankChar(current_char)) {
                // pass

            } else if (Sequence::isLegalSymbol(current_char)) {
                sequence += current_char;

            } else if (verbose) {
                // This errors aren't store since they wont be used latter in this project.
                std::cout << "None invisible symbol ignored at ln " << current_line << " col " << current_column << " : '"
                          << (char)current_char << "' (" << (int)current_char << ")." <<  std::endl;
            }
            

        } else {
            // If this isn't a sequence nor a header, we have nothing to do.
        }
    
        // --- Update <last_char> --- 
        last_char = current_char;
    }

    /* DEBUG block : Display result
    std::cout << header << std::endl;
    std::cout << sequence << std::endl;
    for(const auto& elem : comments) {std::cout << elem.first << " " << elem.second << "\n";}
    */

    return std::tuple<std::string, std::string, std::map<size_t, std::string>> {
                      header,      sequence,    comments};
     

}

std::vector<Fasta> Fasta::loadFastaFromFile(std::string path) {
    return loadFastaFromFile(path, false);
}

std::vector<Fasta> Fasta::loadFastaFromFile(std::string path, bool verbose) {
    std::ifstream file_flux(path);
    unsigned int file_line = 0;
    std::tuple<std::string, std::string, std::map<size_t, std::string>> result_tup;
    std::vector<Fasta> all_fasta;

    while (file_flux) {
        result_tup = parseFasta(file_flux, file_line, verbose);
        Sequence c_seq(
            std::get<1>(result_tup),
            verbose
        );
        c_seq.loadComments(std::get<2>(result_tup));

        all_fasta.insert(all_fasta.end(), Fasta(std::get<0>(result_tup), c_seq));
    }
    return all_fasta;

}

Fasta::Fasta(std::string header, Sequence sequence) {
    this->raw_header = header;
    this->sequence = sequence;
}

/*

Fasta::Fasta(std::string name, bool append) {
    if (append) {
        std::ios_base::app;
    } else {

    }
}*/

