#include "../libpz/include/pz_cxx_std.hpp"
#include "../libpz/include/FMIndex.hpp"

std::vector<int> expected_output(const std::string& filepath) {
    std::vector<int> outputs;
    std::ifstream file(filepath);
    if (!file.is_open()){
        std::cerr << "Error opening expected output file: " << filepath << std::endl;
        exit(1);
    }
    int val;
    while (file>>val){
        outputs.push_back(val);
    }
    return outputs;
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        std::cerr<<"Usage: "<< argv[0]<<" <input_file> <expected_output_file>"<< std::endl;
        return 1;
    }

    std::string input=argv[1];
    std::string out=argv[2];
 
    std::vector<int> expected_outputs=expected_output(out);
    int expected_idx=0;

    std::ifstream input_file(input);
    if (!input_file.is_open()) {
        std::cerr<<"Error opening input file: "<<input<<std::endl;
        return 1;
    }

    std::string line;
    std::string text;
    FMIndex* fm_index=nullptr;
    bool text_pres=0;

    while (std::getline(input_file,line)){
        if(line=="TEXT"){
            text_pres=1;
            text=""; 
            continue;
        }
        
        if(line=="QUERY"){
            if(text_pres){
                auto build_start = std::chrono::high_resolution_clock::now();
                fm_index=new FMIndex(text);
                auto build_end = std::chrono::high_resolution_clock::now(); 
                auto build_duration = std::chrono::duration_cast<std::chrono::microseconds>(build_end-build_start).count();
                std::cout << "Time taken for building fm index : " << build_duration << " us" << std::endl;           
                text_pres=0;
            }
            
            if(!std::getline(input_file,line))break;
            std::string query=line;
            
            auto q_start=std::chrono::high_resolution_clock::now();
            int result=fm_index->count(query, query.length());
            auto q_end=std::chrono::high_resolution_clock::now();
            auto q_duration=std::chrono::duration_cast<std::chrono::microseconds>(q_end-q_start).count();
            
            int expected = expected_outputs[expected_idx];
            expected_idx++;
            
            if (result == expected) {
                std::cout << "Tests passed. Time taken: " << q_duration << " us" << std::endl;
                if(expected_idx == expected_outputs.size()) std::cout << "All tests passed." << std::endl;
            } else {
                std::cout << "False (Expected " << expected << ", Got " << result << ")" << std::endl;
                delete fm_index;
                return 0; 
            }            
            continue;
        }

        if(text_pres){
            text+=line;
        }
    }
    if (fm_index) delete fm_index;
    return 0;
}
