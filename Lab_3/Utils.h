#pragma once
#include<string>
#include<iostream>
#include<unordered_map>
#include<set>
using namespace std;

#define Input_Line(in, str) getline(in>>ws, str); \
						cerr << str << endl

#define PRINT_PARAM(out, var_name, x) out<< var_name << ": " << x << endl

template <typename T>
T Input_Num(string prompt, T min, T max) {
    T input;
    cout << prompt;
    while (!(cin >> input && input >= min && input <= max) ) {   
        cin.clear();
        cin.ignore(2000, '\n');
        cout << "Invalid input. Please try again" << endl;
    }
    cerr << input << endl;
    return input;
} 

template<typename K, typename V>
pair<bool, V&> Find_in_umap(const string& prompt, unordered_map<K, V>& um) {
    static V nothing;
    K key = Input_Num(prompt, numeric_limits<K>::min(), numeric_limits<K>::max());
    if (um.contains(key)) return { true,um[key] };
    else return {false, nothing};
}

template<typename V, typename T>
using Filter = bool(*)(const V& value, const T& param);

template<typename K, typename V, typename T>
set<int> Find_by_filter(const unordered_map<K, V>& um, const Filter<V, T>& fil_res, const T& param)
{
    set<int> search_res;
    for (const auto &[id, obj] : um) 
        if (fil_res(obj, param)) 
            search_res.insert(id);
    
    return search_res;
}

template<typename K, typename V>
void Print_filtered(const unordered_map<K, V>& um, const set<K>& ids) {
    if (ids.empty()) { 
        cout << "Nothing found!\n";
        return;
    }
    for (const auto& id : ids) cout << um.at(id);
}


class redirect_output_wrapper
{
    std::ostream& stream;
    std::streambuf* const old_buf;
public:
    redirect_output_wrapper(std::ostream& src)
        :old_buf(src.rdbuf()), stream(src)
    {
    }

    ~redirect_output_wrapper() {
        stream.rdbuf(old_buf);
    }
    void redirect(std::ostream& dest)
    {
        stream.rdbuf(dest.rdbuf());
    }
};