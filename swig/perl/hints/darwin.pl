# we add libstdc++ otherwise OSX gets a bunch of undefined symbols
$self->{LIBS}->[0] .= ' -lstdc++';
