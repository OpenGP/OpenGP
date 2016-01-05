#pragma once
#include <stdexcept>
#include <string>
#include <map>

//=============================================================================
namespace OpenGP {
//=============================================================================

/// Class to add global properties support to OpenGP objects 
/// @see examples/global_properties.cpp
class Global_properties{
    /// Base
    class Base_global_property{
    public:
        const std::type_info& mytype;
        Base_global_property(const std::type_info& mytype=typeid(void)) 
            : mytype( mytype ){}
    };
    
    /// Templated 
    template <class T>
    class Global_property : public Base_global_property{
    public:
        T value; ///< Properties are stored by copy
        Global_property() : Base_global_property( typeid(T) ){}
    };

private:
    typedef std::map<std::string, Base_global_property*> PropertiesMap;
    PropertiesMap global_props_;
    
public:
    ~Global_properties(){
        // std::cout << global_props_.size() << std::endl;
        for(PropertiesMap::iterator it=global_props_.begin(); it!=global_props_.end(); it++)
            delete (it->second);
    }

public:
    /// Generic 
    template <class T> 
    T& add_property(const std::string& name){
        if(global_props_.find(name) != global_props_.end())
            throw std::runtime_error("Attempted to create property with same name");
        Global_property<T>* prop = new Global_property<T>();
        global_props_[name] = prop;
        return prop->value; ///<return a reference
    }
    
    /// Generic with immediate initialization
    template <class T> 
    T& add_property(const std::string& name, const T& initval){
        T& val = add_property<T>(name);
        val = initval;
        return val;
    }
    
    // get a property by its name. returns invalid property if it does not exist.
    template <class T> 
    T& get_property(const std::string& name){
        if(global_props_.find(name) == global_props_.end())
            throw std::runtime_error("Cannot find property");
        if(global_props_[name]->mytype != typeid(T))
            throw std::runtime_error("Property of desired type not found");
        /// Now static cast is safe            
        Global_property<T>* prop = static_cast< Global_property<T>* >(global_props_[name]);
        return prop->value;
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
