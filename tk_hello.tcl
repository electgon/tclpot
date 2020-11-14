#!/usr/bin/tclsh

package require cmdline
package require struct::matrix

puts "Hello World ! ........"

# This script is copied from (c) mktclapp
# https://www.hwaci.com/sw/mktclapp/mktclapp.html
# ----------------------------------------------------------
button .b -text Go -command Hello
button .e -text Quit -command exit
pack .b .e -side left
proc Hello {} {
  catch {destroy .hi}
  toplevel .hi
  button .hi.b -text {Hello, World!} -command {destroy .hi}
  pack .hi.b
}
# ----------------------------------------------------------