#! perl -w

package xsldbgmatch;

# template list match
sub templateListMatch{
  my ($template, $fileName, $line,  $testName) = @_;
  my $result = 1;
  while (<STDIN>) {
    if ($_ =~ /^ template :\"(.*)\" in file ([^ ]*) : line ([0-9]*)/) {
      if (($template ne "") && ($template ne $1 )) {
	printf "template bad expected \"$template\" found \"$1\"\n";
	$result = 0;
      }

      if (($fileName ne "") && ($fileName ne $2 )) {
	printf "file bad expected \"$fileName\" found \"$2\"\n";
	$result = 0;
      }
      if (($line ne "") && ($line ne $3 )) {
	printf "line bad expected \"$line\" found \"$3\"\n";
	$result = 0;
      }
      if ($result == 1) {
	printf "Template match\n";
      } else {
	printf "In $testName, no match for\n";
      }
      printf "template \"$template\" ";
      printf "file \"$fileName\" ";
      printf "line \"$line\" \n";
      return $result;
    }
  }
  return $result;
}

# a breakpoint that occurs when steping though code
sub breakpointMatch{
  my ($fileName, $line, $testName) = @_;
  my $result = 1;
  while (<STDIN>) {
    if ($_ =~ /^Breakpoint at file ([^ ]*) : line ([0-9]*)/) {
      if (($fileName ne "") && ($fileName ne $1 )) {
	printf "file bad expected \"$fileName\" found \"$1\"\n";
	$result = 0;
      }
      if (($line ne "") && ($line ne $2 )) {
	printf "line bad expected \"$line\"  found \"$2\"\n";
	$result =0;
      }
      if ($result == 1) {
	printf "Breakpoint match\n";
      } else {
	printf "In $testName, no match for\n";
      }
      printf "file \"$fileName\" ";
      printf "line \"$line\" \n";
      return $result;
    }
  }
  return $result;
}

sub breakpointListMatch {
  my ($identifier, $state, $fileName, $line, $testName) = @_;
  my $result = 1;
  while (<STDIN>) {
    # breakpoint list match
    if ($_ =~ /^ Breakpoint ([0-9]*) (enabled|disabled).* file ([^ ]*) : line ([0-9]*)/) {
      if (($identifier ne "") && ($identifier ne $1 )) {
	printf "indentifier bad expected \"$indentifier\" found \"$1\"\n";
	$result = 0;
      }

      if (($state ne "") && ($state ne $2 )) {
	printf "state bad expected \"$state\" found \"$2\"\n";
	$result = 0;
      }

      if (($fileName ne "") && ($fileName ne $3 )) {
	printf "file bad expected \"$fileName\" found \"$3\"\n";
	$result = 0;
      }

      if (($line ne "") && ($line ne $4 )) {
	printf "line bad expected \"$line\" found \"$4\"\n";
	$result = 0;
      }
      if ($result == 1) {
	printf "Breakpoint match\n";
      } else {
	printf "In $testName, no match for\n";
      }
      printf "identifier \"$identifier\", ";
      printf "state \"$state\" ";
      printf "file \"$fileName\" ";
      printf "line \"$line\" \n";
      return $result;
    }
  }
  return $result;
}

# a libxslt parameter
sub parameterMatch{
  my ($id, $name, $value, $testName) = @_;
  my $result = 1;
  while (<STDIN>) {
    if ($_ =~ /^ Parameter ([0-9]*) ([^=]*)=\"(.*)\"/){
      if (($id ne "") && ($id ne $1 )) {
	printf "parameter id bad expected \"$id\" found \"$1\"\n";
	$result = 0;
      }
      if (($name ne "") && ($name ne $2 )) {
	printf "name bad expected \"$name\"  found \"$2\"\n";
	$result =0;
      }
      if (($value ne "") && ($value ne $3 )) {
	printf "value bad expected \"$value\"  found \"$3\"\n";
	$result =0;
      }
      if ($result == 1) {
	printf "Parameter match\n";
      } else {
	printf "In $testName, no match for\n";
      }
      printf "id \"$id\", ";
      printf "name \"$name\", ";
      printf "value \"$value\" \n";
      return $result;
    }
  }
  return $result;
}

sub textMatch{
  my ($textValue, $testName) = @_;
  my $result = 0;
  while (<STDIN>) {
    if ($_ =~ $textValue){
      $result = 1;
      print "Text match \n  text \"$textValue\"\n";
      return $result;
    }
  }
  if ($result == 0){
    printf "In $testName, no match for\n text \"$textValue\"\n";
  }
  return $result;
}
1;
