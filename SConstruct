sources = Glob("src/*.c")
sources += Glob("example/*.c")

env_options = {
     "CPPPATH": ['src'],
}
env = Environment(**env_options)
env.Program("delay", sources)
