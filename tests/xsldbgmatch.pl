#! perl -w

package xsldbgmatch;

# template list match
sub templateListMatch{
    my ($template, $fileName, $line,  $testName) = @_;
    my $result = 1;
    my $errorText = "";
    while (<STDIN>) {
	printf "templateListMatch is looking at $_";
	if ($_ =~ /^ template :\"(.*)\" mode :\"(.*)\" in file ([^ ]*) : line ([0-9]*)/) {
	    if (($template ne "") && ($template ne $1 )) {
		$errorText .= "template bad expected \"$template\" found \"$1\"\n";
		$result = 0;
	    }

	if (($fileName ne "") && ($fileName ne $3 )) {
	    $errorText .= "file bad expected \"$fileName\" found \"$3\"\n";
	    $result = 0;
	}
	if (($line ne "") && ($line ne $4 )) {
	    $errorText .= "line bad expected \"$line\" found \"$4\"\n";
	    $result = 0;
	}
	if ($result == 1) {
	    printf "Template match\n";
	    printf "\ttemplate \"$template\" ";
	    printf "file \"$fileName\" ";
	    printf "line \"$line\" \n";
	} else {
	    printf "In $testName, no match for\n";
	    printf "\ttemplate \"$template\" ";
	    printf "file \"$fileName\" ";
	    printf "line \"$line\" \n";
	    printf "Error is : $errorText\n";
	}
	return $result;
    }
    }
    return $result;
}

# a breakpoint that occurs when steping though code
sub breakpointMatch{
    my ($fileName, $line, $testName) = @_;
    my $result = 1;
    my $errorText = "";
    while (<STDIN>) {
	#printf "breakpointMatch is looking at $_";
	if ($_ =~ /Breakpoint for file \"([^ ]*)\" at line ([0-9]*)/) {
	    if (($fileName ne "") && ($fileName ne $1 )) {
		$errorText .= "file bad expected \"$fileName\" found \"$1\"\n";
		$result = 0;
	    }
	    if (($line ne "") && ($line ne $2 )) {
		$errorText .= "line bad expected \"$line\"  found \"$2\"\n";
		$result =0;
	    }
	    if ($result == 1) {
		printf "Breakpoint match\n";
		printf "\tfile \"$fileName\" ";
		printf "line \"$line\" \n";
	    } else {
		printf "In $testName, no match for\n";
		printf "\tfile \"$fileName\" ";
		printf "line \"$line\" \n";
		printf "Error is : $errorText\n";
	    }
	    return $result;
	}
    }
    printf "breakpointMatch reached end if file looking for $fileName, $line, $testName\n";
    return $result;
}

sub breakpointListMatch {
    my ($identifier, $state, $fileName, $line, $testName) = @_;
    my $result = 1;
    my $errorText = "";
    while (<STDIN>) {
	printf "breakpointListMatch is looking at $_";
# breakpoint list match
	if ($_ =~ /^ Breakpoint ([0-9]*) (enabled|disabled).* file \"([^ ]*)\" at line ([0-9]*)/) {
	    if (($identifier ne "") && ($identifier ne $1 )) {
		$errorText .= "indentifier bad expected \"$indentifier\" found \"$1\"\n";
		$result = 0;
	    }

	    if (($state ne "") && ($state ne $2 )) {
		$errorText .= "state bad expected \"$state\" found \"$2\"\n";
		$result = 0;
	    }

	    if (($fileName ne "") && ($fileName ne $3 )) {
		$errorText .= "file bad expected \"$fileName\" found $3\n";
		$result = 0;
	    }

	    if (($line ne "") && ($line ne $4 )) {
		$errorText .= "line bad expected \"$line\" found \"$4\"\n";
		$result = 0;
	    }
	    if ($result == 1) {
		printf "Breakpoint match\n";
		printf "\tidentifier \"$identifier\", ";
		printf "state \"$state\" ";
		printf "file \"$fileName\" ";
		printf "line \"$line\" \n";
	    } else {
		printf "In $testName, no match for\n";
		printf "\tidentifier \"$identifier\", ";
		printf "state \"$state\" ";
		printf "file \"$fileName\" ";
		printf "line \"$line\" \n";
		printf "Error is : $errorText\n";
	    }
	    return $result;
	}
    }
    return $result;
}

# a libxslt parameter
sub parameterMatch{
    my ($id, $name, $value, $testName) = @_;
    my $result = 1;
    my $errorText = "";
    while (<STDIN>) {
	printf "parameterMatch is looking at $_";
	if ($_ =~ /^ Parameter ([0-9]*) ([^=]*)=\"(.*)\"/){
	    if (($id ne "") && ($id ne $1 )) {
		$errorText .= "parameter id bad expected \"$id\" found \"$1\"\n";
		$result = 0;
	    }
	if (($name ne "") && ($name ne $2 )) {
	    $errorText .= "name bad expected \"$name\"  found \"$2\"\n";
	    $result =0;
	}
	if (($value ne "") && ($value ne $3 )) {
	    $errorText .= "value bad expected \"$value\"  found \"$3\"\n";
	    $result =0;
	}
	if ($result == 1) {
	    printf "Parameter match\n";
	    printf "\tid \"$id\", ";
	    printf "name \"$name\", ";
	    printf "value \"$value\" \n";
	} else {
	    printf "In $testName, no match for\n";
	    printf "\tid \"$id\", ";
	    printf "name \"$name\", ";
	    printf "value \"$value\" \n";
	    printf "Error is : $errorText\n";
	}
	return $result;
    }
    }
    return $result;
}

sub textMatch{
    my ($textValue, $testName) = @_;
    my $result = 0;
    my $firstLine = $_;
    my $errorText = "";
    while (<STDIN>) {
	if ($_ =~ /$textValue/){
	    $result = 1;
	    print "Text match \n text \"$textValue\" in $_\n";
	    return $result;
	}else{
	    printf "looking at : $_\n";
	}
    }
    if ($result == 0){
	printf "In $testName, no match for\n text \"$textValue\"\n";
	printf "Near text: $firstLine\n";
    }
    return $result;
}
1;
