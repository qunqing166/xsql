// #include <TableBase.h>
//
// namespace x{
//
// template <typename T>
// template
// constexpr void TableBase::Set(const std::string& name, const std::string& value)
// {
//     auto it = m_table.find(name);
//     if(it == m_table.end())return;
//
//     using vt = std::invoke_result_t<decltype(it->second.first), T*>;
//
//     if constexpr (std::is_convertible_v<vt, std::string>) {
//         it->second.second((T*)this, value);
//     }else if constexpr (std::is_integral_v<vt>){  
//         it->second.second((T*)this, std::stoll(value));
//     }else if constexpr (std::is_floating_point_v<vt>) {
//         it->second.second((T*)this, std::stod(value));
//     }
// }
//
// }
