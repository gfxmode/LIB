var gulp = require('gulp');
var gulpSequence = require('gulp-sequence');
var jshint = require('gulp-jshint');
var concat = require('gulp-concat');
var path = require('path');
var del = require('del');

var destFolder = 'dist';
var destFileName = 'Util.js';

gulp.task('clean', function (cb){
    return del([destFolder + '/**/*'], cb);
});

gulp.task('jshint', function(){
   gulp.src('src/*/*.js')
      .pipe(jshint())
      .pipe(jshint.reporter('fail'));
});

gulp.task('concat',function(){
    gulp.src(['src/module.js', 'src/**/*.js'])
    .pipe(concat(destFileName))
    .pipe(gulp.dest(destFolder));
});

gulp.task('all', gulpSequence('clean', 'jshint', 'concat'));

gulp.task('default', ['all'], function(){});
