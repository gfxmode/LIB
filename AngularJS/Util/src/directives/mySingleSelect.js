(function(){
    'use strict';
    angular.module('Util')
    .directive('mySingleSelect', function(){
        var retObj;
        retObj = {
            restrict: 'E',
            scope: {
                id: '@',
                values: '='
            },
            replace: true,
            template: '<select ng-model="selectedItem"><option ng-repeat="x in values" value="{{x.value}}">{{x.label}}</option></select>',
            link: function(scope, iEle, iAttr){
                var gScope = scope;
                console.log(gScope);
                var getValue = function() {
                    console.log(gScope);
                };

                var setValue = function() {
                    console.log("setValue");
                };

                if(scope.$parent.id){
                    scope.$parent[scope.id] = {
                        getValue: getValue,
                        setValue: setValue
                    };
                }
            }
        };
        return retObj;
    });
}());