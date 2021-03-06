#!/usr/bin/perl

#       $Id$
 
#  (C) Copyright IBM Corp. 2004
 
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
#  file and program are licensed under a BSD style license.  See
#  the Copying file included with the OpenHPI distribution for
#  full licensing terms.
 
#  Authors:
#      Sean Dague <http://dague.net/sean>

use strict;
use HTML::Entities;

foreach my $file (@ARGV) {
#    my $outfile = $file . ".html";
    open(IN,"$file");
    my @lines = <IN>;
    close(IN);
    
    my $html = make_html_head($file);
    $html .= make_html_body(@lines);
    $html .= make_html_tail();
    
    if(scalar(@lines) > 5) {
        print $html;
    }
#    open(OUT,">$outfile");
#    print OUT $html;
#    close(OUT);
}

sub make_html_head {
    my $title = shift;
    $title =~ s/.*\/(.*)\.summary$/$1/;
    return <<END;
<html>
<head><title>GCOV Report for $title</title>
<link rel="stylesheet" href="/openhpi.css" type="text/css">
</head>
<body>
<div id="banner"><div><h1>The OpenHPI Project</h1><small>Open Hardware Platform Interface</small></div></div>
<table>
<tr>
<!--#include virtual="/sidebar.html" -->
<td id="maincolumn"><div class="mainsegment">
<h3>GCOV Summary for $title</h3>
<div>
END
}

sub make_html_tail {
    return <<END;
</table></div>
</div></td></tr></table>
</body>
</html>
END
}

sub set_status {
    my $exec = shift;
    if($exec eq "-") {
        return "na";
    } elsif($exec eq "#####") {
        return "notexec";
    } elsif($exec < 10) {
        return "low";
    } else {
        return "good";
    }
}

sub make_html_body {
    my @lines = @_;
    my $html;
    foreach my $line (@lines) {
        if($line =~ /^Function/) {
            $line =~ s/.*`([^']+).*/$1/;
            # close the last table
            $html .= "</table>\n";
            $html .= "<h4 class='function'>$line</h4>\n<table class='report'>\n";
        } elsif($line =~ /^File/) {
            $line =~ s/.*`([^']+).*/$1/;
            $html .= "<h4 class='file'>$line</h4>\n<table class='report'>\n"
        } elsif ($line =~ /^No/) {
            $html .= "<tr class='na'><td>$line</td></tr>\n";
        } elsif ($line =~ /(\d+\.\d{2})%/) {
            my $per = $1;
            my $status = "bad";
            if($per >= 100) {
                $status = "great";
            } elsif($per > 80) {
                $status = "good";
            } elsif($per > 50) {
                $status = "ok";
            }
            $html .= "<tr class='$status'><td>$line</td></tr>\n";
        }
    }
    return $html;
}
