(function(){
    'use strict';
    angular.module('Util').factory("DataService", function(){
        var f = {};

        f.set_local_storage = function(key, value){
            localStorage[key] = JSON.stringify(value);
        };

        f.get_local_storage = function(key){
            var ret;
            if(localStorage[key]){
                ret = JSON.parse(localStorage[key]);
            }
            return ret;
        };
        
        f.set_session_storage = function(key, value){
            sessionStorage[key] = JSON.stringify(value);
        };

        f.get_session_storage = function(key){
            var ret;
            if(sessionStorage[key]){
                ret = JSON.parse(sessionStorage[key]);
            }
            return ret;
        };
        
        return f;
    });
}());