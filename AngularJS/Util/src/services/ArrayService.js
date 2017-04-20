(function(){
    'use strict';
    angular.module('Util').factory('ArrayService', function(){
        var f = {};

        f.randomSort = function (a, b) {
            return Math.random() - 0.5;
        };

        return f;
    });
}());