#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <string.h>
#include <algorithm>

class Trebuchet
{
public:
    Trebuchet() : sumCalibrationValues(0)
        {};
    void CalcSumCalibrationValues();
    unsigned long int getCalibrationValue() const {
        return sumCalibrationValues;
    };

private:
    std::vector<std::pair<int,int>> getRowDigits(std::string& line);
    unsigned long int calcSumOfRowsDigits(std::vector<std::pair<int,int>>& rowDigits);

    std::vector<std::pair<char*,int>> validDigits = {
        /* digit search, digit integer value */
        {(char*)"1", 1}, {(char*)"2", 2}, {(char*)"3", 3}, {(char*)"4", 4}, {(char*)"5", 5}, {(char*)"6", 6}, {(char*)"7", 7}, {(char*)"8", 8}, {(char*)"9", 9},
        {(char*)"one", 1}, {(char*)"two", 2}, {(char*)"three", 3}, {(char*)"four", 4}, {(char*)"five", 5}, {(char*)"six", 6}, {(char*)"seven", 7}, {(char*)"eight", 8}, {(char*)"nine", 9}
    };
    std::ifstream inputFile{"input.txt"};
    // std::ifstream inputFile{"test.txt"};
    std::ofstream outputFile{"calibration.txt"};
    unsigned long int sumCalibrationValues;
};

/**
 * @brief Get all digits in a line/row from input text file
 * 
 * @param line from input file
 * @return std::vector<int,int> digit value & digit position in line
 */
std::vector<std::pair<int,int>> Trebuchet::getRowDigits(std::string& line)
{
    std::vector<std::pair<int,int>> rowDigitsFound = {};
    size_t start_search_pos = 0;
    // std::cout << "Line Length: " << line.length() << "\n";
    const char* arr = line.c_str();
    // std::cout << "number of elements in list: " << validDigits.size() << "\n";

    /* Iterate through line by digit length until all matches are found */
    while(start_search_pos < line.length())
    {
        for (auto it = validDigits.begin(); it != validDigits.end(); it++)
        {
            /* go through line and try all digits for match */
            size_t search_length = strlen(it->first);
            // std::cout << "trying to find: " << it->first << "\tsearch pos: " << start_search_pos << "\tNum chars: " << search_length << "\n";
    
            /* Stay within line parameters, don't compare if would go outside because search is too long */
            if(search_length <= (line.length() - start_search_pos + 1)) {
                if(strncmp(&arr[start_search_pos], it->first, search_length) == 0) //match found
                {
                    rowDigitsFound.push_back({it->second, start_search_pos});
                    // start_search_pos += search_length; // :( this should've worked rather than below
                    start_search_pos += 1; //go through by character since eightwo should count as 82 instead of just 8
                    break;
                }
            }
            else {
                // std::cout << "SEARCHING IS TOO LONG\n";
            }

            /* iterator on last in vector, move onto next next character */
            if(it == validDigits.begin() + (validDigits.size() - 1)) {
                start_search_pos += 1;
                // std::cout << "NO MATCH, MOVING ON NOW\n" << line << "\t";
            }
        }
    }
    return rowDigitsFound;
}

/**
 * @brief Get 'integer' value for line (first and last 'digit' of line) and add to running sum
 * 
 * @param rowDigits - list of all digits found in line
 * @return unsigned long int - sum of first and last digit
 */
unsigned long int Trebuchet::calcSumOfRowsDigits(std::vector<std::pair<int,int>>& rowDigits)
{
    auto rowDigitValue = 0;
    if(rowDigits.size() == 1) { //if only 1 digit in line, ones place
        rowDigitValue = (10*rowDigits.front().first) + rowDigits.front().first;
        // std::cout << "Only 1 digit in line: " << rowDigitValue << "\n";
    }
    else if (rowDigits.size() >= 2) { //if 2+ digits in line, only use 1st and last digits
        auto tens = rowDigits.front().first;
        auto ones = rowDigits.back().first;
        rowDigitValue = (tens * 10) + ones;
        // std::cout << "2+ digits in line: " << rowDigitValue << "\n";
    }
    else{} //do nothing if no digits in line

    outputFile << rowDigitValue << "\n"; //put digits in file for each line

    return sumCalibrationValues += rowDigitValue;
}

/**
 * @brief Calculate the total sum of calibration values for the input file
 * 
 */
void Trebuchet::CalcSumCalibrationValues()
{
    std::string line;
    while (std::getline(inputFile, line))
    {
        // std::cout << line << "\n";

        /* find each digit in line and add valid ones to vector */
        auto lineDigits = getRowDigits(line);
        // for (auto entry : lineDigits)
        // {
        //     std::cout << "digit val: " << entry.first << "\t position found: " << entry.second << "\n";
        // }

        calcSumOfRowsDigits(lineDigits);
        // std::cout << "current VALUE: " << sumCalibrationValues << "\n";
    }
}


int main() {
    Trebuchet calibratedTrebuchet;

    calibratedTrebuchet.CalcSumCalibrationValues();

    auto myCalibrationVal = calibratedTrebuchet.getCalibrationValue();
    std::cout << "FINAL VALUE: " << myCalibrationVal;

    return 0;
}