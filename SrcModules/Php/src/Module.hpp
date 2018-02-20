#ifndef MODULE_PHP_HPP_
# define MODULE_PHP_HPP_

#include "api.h"

namespace nzm
{
    class Module : public zia::api::Module
    {
        public:
            Module();
            ~Module();

        /**
        * Called after contruct and when config changed.
        * \return true on success, otherwise false.
        */
        bool config(const zia::api::Conf& conf);

        /**
        * Called on HTTP request.
        * \return true on success, otherwise false.
        */
        bool exec(zia::api::HttpDuplex& http);

        zia::api::Module* create();
    };
}

#endif