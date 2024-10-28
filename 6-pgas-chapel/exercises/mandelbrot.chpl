/*
 * Author: Guillaume HELBECQUE (Université du Luxembourg)
 * Date: 26/10/2024
 *
 * Description:
 * This sequential program computes a Mandelbrot set of given size and
 * resolution using Chapel. It serves as a basis for parallel implementations.
 */

use IO, Time;

// parameters (image frame)
param x_min = -1.78;
param x_max =  0.78;
param y_min = -0.961;
param y_max =  0.961;

// configurable options
config const N: int       = 100;  // number of iteration
config const width: int   = 1024; // number of horizontal pixels
config const height: int  = 768;  // number of vertical pixels
config const path: string = "./mandelbrot_seq";

// "image" type
record image {
  var nb_rows, nb_columns: int; // number of pixels
  var dom: domain(1);
  var pixels: [dom] int; // linearized matrix of pixels

  proc init(nb_rows: int, nb_columns: int) {
    this.nb_rows = nb_rows;
    this.nb_columns = nb_columns;
    this.dom = {0..nb_rows*nb_columns-1};
  }
}

// save a given image in .ppm format
proc save_image(const ref im: image) {
  var tup = (path,"_",im.nb_rows:string,"_",im.nb_columns:string,"_",N:string,".ppm");
  var newpath = "".join(tup);

  var f = open(newpath, ioMode.cw);
  var channel = f.writer();

  var c: int;
  channel.writef("P6\n%i %i\n127\n", im.nb_columns, im.nb_rows);
  for i in im.dom {
    c = im.pixels[i];
    channel.writef("%c%c%c", c, c, c); // monochrome weight
  }

  channel.close();
  f.close();
}

// sequential computation of the Mandelbrot set
proc compute(ref im: image) {
  const dx: real = (x_max - x_min) / im.nb_columns;
  const dy: real = (y_max - y_min) / im.nb_rows;

  for l in 0..im.nb_rows-1 {
    for c in 0..im.nb_columns-1 {

      var a: real = x_min + c * dx;
      var b: real = y_max - l * dy;
      var x: real = 0.0;
      var y: real = 0.0;
      var i: int = 0;

      while (i < N) {
        var tmp: real = x;
        x = x*x - y*y + a;
        y = 2 * tmp * y + b;
        if (x*x + y*y > 4) {
          break; // divergence
        }
        else {
          i += 1;
        }
      }

      im.pixels[c + l * im.nb_columns] = (i:real / N * 127):int;
    }
  }
}

proc main() {
  var im = new image(height, width);
  var timer: stopwatch;

  timer.start();
  compute(im);
  timer.stop();

  writeln("Computation of Manderbrot set with ", N, " iterations and ",
    height, " x ", width, " resolutions");
  writeln("Elapsed time [s]: ", timer.elapsed());

  save_image(im);

  return 0;
}
