(function () {
    'use strict';
    angular.module('Util').factory('SearchScope', function () {
        return function (scope, id) {
            var ret = {};
            if (scope.id === id) {
                return scope;
            }
            var root = scope;
            var pendingChildHeads = [root.$$childHead];
            var currentScope;

            while (pendingChildHeads.length) {
                currentScope = pendingChildHeads.shift();

                while (currentScope) {
                    if (currentScope.id === id) {
                        return currentScope;
                    }
                    pendingChildHeads.push(currentScope.$$childHead);
                    currentScope = currentScope.$$nextSibling;
                }
            }
        };
    });
}());