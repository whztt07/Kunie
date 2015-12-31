TEMPLATE = subdirs
SUBDIRS = core app test

core.file = core/core.pro

app.file = app/Kunie.pro
app.depends = core

test.file = test/test.pro
test.depends = core
