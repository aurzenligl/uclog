import os
import sys
from waflib import Options, Tools

def options(ctx):
    ctx.load('compiler_cxx waf_unit_test')
    ctx.add_option('--perf', action='store_true', help='Run bprintf performance test.')

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
        cxxflags = '-g -Wall -O3 -std=c++11',
        use = 'uclog')

    ctx.add_post_fun(Tools.waf_unit_test.summary)
    ctx.add_post_fun(Tools.waf_unit_test.set_exit_code)

    if getattr(Options.options, 'perf'):
        ctx(rule = lambda task: os.system(task.inputs[0].abspath()),
            source = 'perftest',
            always = True)
