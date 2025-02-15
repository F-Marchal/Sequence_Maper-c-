#include "FastaHeader.hpp"
#include "Global/Utilities.hpp"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <tuple>
#include <vector>


FastaHeader::FastaHeader(std::string raw_header) {
    displayLogicError(raise, "Unimplemented class. This class is not implemented yet.", __FILE__, __func__);
    // SEE parseRawHeader
    this->load(raw_header);
}   

void FastaHeader::load(std::string raw_header) {
    std::map<std::string, std::string> map_to_load= parseRawHeader(raw_header);
    this->identifier.clear();
    this->identifier.insert(map_to_load.begin(), map_to_load.end());
    this->setComment(map_to_load["Comments"]);
    this->setAccession(map_to_load["accession"]);
    this->identifier_format = map_to_load["Format"];
}

const std::map<std::string, std::string> & FastaHeader::getIdentifiers() {
    return this->identifier;
}

std::string FastaHeader::getFormat() {
    return this->identifier_format;
}

// ---------------------------------------------------------------------------------------------
void FastaHeader::clearRawHeader(std::string& raw_header) {
    size_t i = 0;
    while (i < raw_header.size() && (isHeaderChar(raw_header[i]) || isBlankChar(raw_header[i]))){
        i += 1;
    }

    if (i != 0) {
        raw_header.erase(0, i);
    }
}

size_t FastaHeader::findFirstWord(std::string text) {
    if (text.size() == 0) {
        return 0;
    }

    size_t i = 0;
    while (i < text.size() && !isBlankChar(text[i])) {
        i += 1; 
    }
    
    return i;
}

std::vector<std::string> FastaHeader::cutRawHeader(std::string raw_header) {
    std::vector<std::string> text_subparts;
    std::string current_part = "";

    clearRawHeader(raw_header);
    for (char symbols : raw_header) {
        if (symbols == '|') {
            text_subparts.push_back(current_part);
            current_part = "";
        } else {
            current_part += symbols;
        }
        
    }
    text_subparts.push_back(current_part);
    
    return text_subparts;
 }



std::map<std::string, std::string> FastaHeader::parseRawHeader(std::vector<std::string> cut_raw_header) {
    std::map<std::string, std::string> final_map;

    std::string format;
    format = cut_raw_header[0];
    if (format.size() == 1) {
        std::string firstWord = format;
        final_map["Complete"] = firstWord + " ";
        final_map["Format"] = "?";
        final_map["Comments"] = cut_raw_header[0].substr(firstWord.size());
        final_map["accession"] = firstWord;

    } else  {
        //TODO: Determine sequence type (gb...)
        //TODO: Call the right function (make{identifier}Header) ()
    }
    return final_map;
};

std::map<std::string, std::string> FastaHeader::parseRawHeader(std::string raw_header) {return parseRawHeader(cutRawHeader(raw_header));}

// 
std::tuple<std::string, std::string> FastaHeader::intermediateMakeFormatFromVector(std::vector<std::string> text_header, 
    unsigned int expected_size, std::string format) {
    // Factorise the commun part of functions
    if (expected_size <= 1) {
         throw std::invalid_argument("expected_size should be greater or equal to 2. Got : " + std::to_string(expected_size));
    }

    if (text_header.size() != expected_size) {
        throw std::invalid_argument(std::to_string(expected_size) + " items are expected in order to parse a '" + format + "' header. Got : " + std::to_string(text_header.size()));
    
    } else if (text_header[0] != format) {
        throw std::invalid_argument("Wrong format identifier expected : '" + format + "' got : '" + text_header[0] + "'.");
    }

    size_t split_point = findFirstWord(text_header[expected_size - 1]);
    return std::tuple<std::string, std::string> {text_header[-1].substr(0, split_point), text_header[expected_size - 1].substr(split_point)};
}



// --- --- ---- --- --- --- --- --- 2 --- --- ---- --- --- --- --- --- 
// --- ---- ---- ---- --- GenInfoBackboneId --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeGenInfoBackboneIdHeader(std::string accession, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "bbs|" + accession;
    final_map["Format"] = "bbs";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseGenInfoBackboneIdHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 2, "bbs");
    return makeGenInfoBackboneIdHeader(std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseGenInfoBackboneIdHeader(std::string raw_header) {
    return parseGenInfoBackboneIdHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- GenInfoBackboneMoltype --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeGenInfoBackboneMoltypeHeader(std::string accession, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "bbm|" + accession;
    final_map["Format"] = "bbm";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseGenInfoBackboneMoltypeHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 2, "bbm");
    return makeGenInfoBackboneMoltypeHeader(std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseGenInfoBackboneMoltypeHeader(std::string raw_header) {
    return parseGenInfoBackboneMoltypeHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- GenInfoBackboneMoltype --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeGenInfoImportIdHeader(std::string accession, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "gim|" + accession;
    final_map["Format"] = "gim";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseGenInfoImportIdHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 2, "gim");
    return makeGenInfoImportIdHeader(std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseGenInfoImportIdHeader(std::string raw_header) {
    return parseGenInfoImportIdHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- GenInfoIntegratedDatabase --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeGenInfoIntegratedDatabaseHeader(std::string accession, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "gi|" + accession;
    final_map["Format"] = "gi";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseGenInfoIntegratedDatabaseHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 2, "gi");
    return makeGenInfoIntegratedDatabaseHeader(std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseGenInfoIntegratedDatabaseHeader(std::string raw_header) {
    return parseGenInfoIntegratedDatabaseHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- LocalSequenceIdentifier --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeLocalSequenceIdentifierHeader(std::string accession, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "lcl|" + accession;
    final_map["Format"] = "lcl";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseLocalSequenceIdentifierHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 2, "lcl");
    return makeLocalSequenceIdentifierHeader(std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseLocalSequenceIdentifierHeader(std::string raw_header) {
    return parseLocalSequenceIdentifierHeader(cutRawHeader(raw_header));
}


// --- --- ---- --- --- --- --- --- 3 --- --- ---- --- --- --- --- --- 
// --- ---- ---- ---- --- GenBank --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeGenbankHeader(std::string accession, std::string locus, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "gb|" + accession + "|" + locus;
    final_map["Format"] = "gb";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["locus"] = locus;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseGenbankHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "gb");
    return makePIRHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseGenbankHeader(std::string raw_header) {
    return parseGenbankHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- EMBL --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makePIRHeader(std::string accession, std::string locus, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "gb|" + accession + "|" + locus;
    final_map["Format"] = "gb";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["locus"] = locus;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parsePIRHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "gb");
    return makePIRHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parsePIRHeader(std::string raw_header) {
    return parsePIRHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- EMBL --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeEMBLHeader(std::string accession, std::string name, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "emb|" + accession + "|" + name;
    final_map["Format"] = "emb";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["name"] = name;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseEMBLHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "emb");
    return makeEMBLHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseEMBLHeader(std::string raw_header) {
    return parseEMBLHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- PIF --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makePIFHeader(std::string accession, std::string name, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "pif|" + accession + "|" + name;
    final_map["Format"] = "pif";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["name"] = name;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parsePIFHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "pif");
    return makePIFHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parsePIFHeader(std::string raw_header) {
    return parsePIFHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- SwissProt --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeSwissProtHeader(std::string accession, std::string name, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "sp|" + accession + "|" + name;
    final_map["Format"] = "sp";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["name"] = name;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseSwissProtHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "sp");
    return makeSwissProtHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseSwissProtHeader(std::string raw_header) {
    return parseSwissProtHeader(cutRawHeader(raw_header));
}


// --- ---- ---- ---- --- TrEMBL --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeTrEMBLHeader(std::string accession, std::string name, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "tr|" + accession + "|" + name;
    final_map["Format"] = "tr";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["name"] = name;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseTrEMBLHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "tr");
    return makeTrEMBLHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseTrEMBLHeader(std::string raw_header) {
    return parseTrEMBLHeader(cutRawHeader(raw_header));
}


// --- ---- ---- ---- --- ThirdPartyGenbank --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeThirdPartyGenbankHeader(std::string accession, std::string name, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "tpg|" + accession + "|" + name;
    final_map["Format"] = "tpg";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["name"] = name;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseThirdPartyGenbankHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "tpg");
    return makeThirdPartyGenbankHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseThirdPartyGenbankHeader(std::string raw_header) {
    return parseThirdPartyGenbankHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- ThirdPartyEMBL --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeThirdPartyEMBLHeader(std::string accession, std::string name, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "tpe|" + accession + "|" + name;
    final_map["Format"] = "tpe";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["name"] = name;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseThirdPartyEMBLHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "tpe");
    return makeThirdPartyEMBLHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseThirdPartyEMBLHeader(std::string raw_header) {
    return parseThirdPartyEMBLHeader(cutRawHeader(raw_header));
}


// --- ---- ---- ---- --- ThirdPartyDDBJ --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeThirdPartyDDBJHeader(std::string accession, std::string name, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "tpd|" + accession + "|" + name;
    final_map["Format"] = "tpd";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["name"] = name;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseThirdPartyDDBJHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "tpd");
    return makeThirdPartyDDBJHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseThirdPartyDDBJHeader(std::string raw_header) {
    return parseThirdPartyDDBJHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- PDB --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makePDBHeader(std::string accession, std::string name, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "pdb|" + accession + "|" + name;
    final_map["Format"] = "pdb";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["name"] = name;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parsePDBHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "pdb");
    return makePDBHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parsePDBHeader(std::string raw_header) {
    return parsePDBHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- GNL --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeGNLHeader(std::string accession, std::string name, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "gnl|" + accession + "|" + name;
    final_map["Format"] = "gnl";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["name"] = name;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseGNLHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "gnl");
    return makeGNLHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseGNLHeader(std::string raw_header) {
    return parseGNLHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- NCBIRefSeq --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeNCBIRefSeqHeader(std::string accession, std::string name, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "ref|" + accession + "|" + name;
    final_map["Format"] = "ref";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["name"] = name;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseNCBIRefSeqHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "ref");
    return makeNCBIRefSeqHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseNCBIRefSeqHeader(std::string raw_header) {
    return parseNCBIRefSeqHeader(cutRawHeader(raw_header));
}


// --- ---- ---- ---- --- PatentS --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makePatentSHeader(std::string accession, std::string name, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "pat|" + accession + "|" + name;
    final_map["Format"] = "pat";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["name"] = name;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parsePatentSHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "pat");
    return makePatentSHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parsePatentSHeader(std::string raw_header) {
    return parsePatentSHeader(cutRawHeader(raw_header));
}

// --- ---- ---- ---- --- DDBJ --- ---- ---- ---- --- 
std::map<std::string, std::string> FastaHeader::makeDDBJHeader(std::string accession, std::string name, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "dbj|" + accession + "|" + name;
    final_map["Format"] = "dbj";
    final_map["Comments"] = comments;
    final_map["accession"] = accession;
    final_map["name"] = name;

    return final_map;
}

std::map<std::string, std::string> FastaHeader::parseDDBJHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 3, "dbj");
    return makeDDBJHeader(text_header[1], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parseDDBJHeader(std::string raw_header) {
    return parseDDBJHeader(cutRawHeader(raw_header));
}

// --- --- ---- --- --- --- --- --- 4 --- --- ---- --- --- --- --- ---

std::map<std::string, std::string> FastaHeader::makePatentLHeader(std::string country, std::string patent, std::string sequence_number, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "pat|" + country + "|" + patent + "|" + sequence_number;
    final_map["Format"] = "pat";
    final_map["Comments"] = comments;
    final_map["country"] = country;
    final_map["patent"] = patent;
    final_map["sequence_number"] = sequence_number;
    return final_map;
}

std::map<std::string, std::string> FastaHeader::parsePatentLHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 4, "pat");
    return makePatentLHeader(text_header[1], text_header[2], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parsePatentLHeader(std::string raw_header) {
    return parsePatentLHeader(cutRawHeader(raw_header));
}


std::map<std::string, std::string> FastaHeader::makePreGrantPatentHeader(std::string country, std::string application_number, std::string sequence_number, std::string comments) {
    std::map<std::string, std::string> final_map;
    final_map["Complete"] = "pgp|" + country + "|" + application_number + "|" + sequence_number;
    final_map["Format"] = "pgp";
    final_map["Comments"] = comments;
    final_map["country"] = country;
    final_map["application_number"] = application_number;
    final_map["sequence_number"] = sequence_number;
    return final_map;
}

std::map<std::string, std::string> FastaHeader::parsePreGrantPatentHeader(std::vector<std::string> text_header) {
    std::tuple<std::string, std::string> temp_tup = intermediateMakeFormatFromVector(text_header, 4, "pgp");
    return makePreGrantPatentHeader(text_header[1], text_header[2], std::get<0>(temp_tup), std::get<1>(temp_tup));
}

std::map<std::string, std::string> FastaHeader::parsePreGrantPatentHeader(std::string raw_header) {
    return parsePreGrantPatentHeader(cutRawHeader(raw_header));
}

// --- --- ---- --- --- --- --- --- 5 --- --- ---- --- --- --- --- ---
