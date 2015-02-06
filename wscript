import sys

from waflib.Tools import waf_unit_test

def options(ctx):
    ctx.load('compiler_cxx waf_unit_test')

def configure(ctx):
    ctx.load('compiler_cxx waf_unit_test')

def build(ctx):
    ctx(features = 'cxx',
        target = 'uclog',
        includes = 'include',
        export_includes = 'include',
        source = ctx.path.ant_glob('src/*.cpp'),
        cxxflags = '-g -Wall -O3')

    ctx(features = 'cxx cxxprogram test',
        target = 'test_uclog',
        source = ctx.path.ant_glob('test/*.cpp'),
        cxxflags = '-g -Wall -std=c++11',
        lib = 'gtest pthread',
        use = 'uclog')

    ctx(features = 'cxx cxxprogram',
        target = 'perftest',
        source = ctx.path.ant_glob('test/perf/*.cpp'),
        cxxflags = '-g -Wall -O3',
        use = 'uclog')

    ctx.add_post_fun(waf_unit_test.summary)
    ctx.add_post_fun(waf_unit_test.set_exit_code)

def perf(ctx):
    import os
    import waflib
    os.system(waflib.Context.out_dir + '/perftest')
