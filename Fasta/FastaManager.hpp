#ifndef FASTAMANAGER_HPP
#define FASTAMANAGER_HPP
#include "FastaRelated.hpp"
#include "FastaHeader.hpp"
#include "FastaSequence.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <stdexcept>
#include <set>
#include <vector>
/**
 * @brief Class that should represent a Fasta File. 
 * @warning Not implemented
 */
class FastaManager : FastaRelated{
public:
    /**
     * @brief Load a fasta using it's content
     * @warning NotImplemented
     */
    void loadFile(std::string path);
    /**
     * @brief Load a fasta using it's content
     * @warning NotImplemented
     */
    void loadText(const std::string & file_content);
};

#endif
// class Fasta : public FastaRelated{
// private:
//     FastaSequence sequence;
//     FastaHeader header;



//     static std::map<std::string, std::tuple<char, bool>> extension_types;


// public:
//     static std::tuple<std::string, std::string, std::map<size_t, std::string>>
//     parseFasta(std::ifstream& file_flux, unsigned int& current_line, bool verbose);

//     static std::vector<Fasta> loadFastaFromFile(std::string path);
//     static std::vector<Fasta> loadFastaFromFile(std::string path, bool verbose);
    
//     std::string getCleanSequence(unsigned int word_size, unsigned int paragraph_size) {return getCleanSequence(word_size, paragraph_size, false);};
//     std::string getCleanSequence(unsigned int word_size, unsigned int paragraph_size, bool comments);

//     FastaHeader getHeader() {return this->header;}



//     Fasta(FastaHeader header, FastaSequence& sequence);
//     FastaSequence& getSequence() {return this->sequence;}
    

//     // TODO: Write Files
//     // static void writeFasta(vector<Fasta>)
//     /*
//     void write(std::string path);
//     void write(std::string path, bool append, bool comments);
//     void write(std::string path, unsigned int word_size, unsigned int paragraph_size);
//     void write(std::string path, unsigned int word_size, unsigned int paragraph_size, bool comments);
//     */
//     void write(std::string path, bool append, unsigned int word_size, unsigned int paragraph_size, bool comments);
//     void write(std::vector<Fasta> fasta_vector, std::string path, unsigned int word_size=10, unsigned int paragraph_size=6, bool comments=true);

    
// };
// // TODO: Manage file extension
// #endif