#include "SqlValue.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <mysql/field_types.h>
#include <string>

namespace x{

FieldMeta::FieldMeta(const std::string& name, const std::string& type):
    m_name(name), m_typeStr(type){
    std::transform( m_typeStr.begin(), m_typeStr.end(), 
                    m_typeStr.begin(), ::tolower);
    if(m_typeStr.find("bigint") != std::string::npos){
        m_type = TYPE_BIT_64;
    }else if(m_typeStr.find("tinyint") != std::string::npos){
        m_type = TYPE_BIT_8;
    }else if(m_typeStr.find("smallint")){
        m_type = TYPE_BIT_16;
    }else if(m_typeStr.find("int") != std::string::npos){
        m_type = TYPE_BIT_32;
    }else if(m_typeStr.find("float") != std::string::npos){
        m_type = TYPE_FLOAT;
    }else if(m_typeStr.find("double") != std::string::npos){
        m_type = TYPE_DOUBLE;
    }else{
        m_type = TYPE_STRING;
    }
}


void FieldMeta::InsertType(const std::string& field, const std::string& typeStr){
    std::string str = typeStr;
    std::string fstr = field;
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::transform(fstr.begin(), fstr.end(), fstr.begin(), ::tolower);

    Type type;
    if(str.find("bigint") != std::string::npos){
        type = TYPE_BIT_64;
    }else if(str.find("tinyint") != std::string::npos){
        type = TYPE_BIT_8;
    }else if(str.find("smallint")){
        type = TYPE_BIT_16;
    }else if(str.find("int") != std::string::npos){
        type = TYPE_BIT_32;
    }else if(str.find("float") != std::string::npos){
        type = TYPE_FLOAT;
    }else if(str.find("double") != std::string::npos){
        type = TYPE_DOUBLE;
    }else{
        type = TYPE_STRING;
    }
    m_fieldType[fstr] = type;
}

FieldMeta::Type FieldMeta::GetType(const std::string& field){
    std::string str = field;
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    auto it = m_fieldType.find(str);
    if(it == m_fieldType.end())return FieldMeta::TYPE_UNKOWN;
    return it->second;
}

}
