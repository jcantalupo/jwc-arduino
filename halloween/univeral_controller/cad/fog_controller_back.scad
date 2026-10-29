// 12/1/24 - Tested compomnent size and spacing and it is good.
// Next steps:
//  - Add labels? No, do those with color.
//  - Holes for Nano mount - test
//  - Hole for Nano USB
//  - Holes to mount back plate

hr = 5;          // Corner radius of box
hull_fn = 32;  // fn for box corner

c = 25.4;
b = .5 * c;
x1 = .45 * c;
x2 = .7 * c;
x3 = .2 * c;   // Second row M3
y1 = .3 * c;
y2 = .6 * c;
y3 = .9 * c;  // Second row M3
y4 = 18;     // switches and pots

xt = b + x1 + 4*x2 + x1 + b - 2*hr;
yt = b + y1 + y2 + y4 + b - 2*hr  + 5 + 30 + b;

//m3 = 3.5;
scr = 2.8;   // mounting screw hole 2.6 is too small. 1/8" (real, 3.18mm) is too big
bd = 12;   // button diameter

fpt1 = 6; // front piece thickness 18 should be ok
fpt2 = 5; // front plate thickness

bbh = 8; // button board mounting height 
nh  = 5; // nano mounting height

m3id = 4;  // M3 insert diameter
m3ih = 6;  // M3 insert height

module m3i(td){
    difference() {
        cylinder(d=m3id+3, h=td, $fn=32);
        translate([0,0,td-m3ih]) cylinder(d=m3id, h=m3ih, $fn=32);
    }
}

module boxtop(btX,btY,btZ) {
translate([hr,hr,0])
    hull() {
        cylinder(r=hr, btZ, $fn = hull_fn);
        translate([btX,0,0]) cylinder(r=hr, btZ, $fn = hull_fn);
        translate([btX,btY,0]) cylinder(r=hr, btZ, $fn = hull_fn);
        translate([0,btY,0]) cylinder(r=hr, btZ, $fn = hull_fn);
    }
}

module m3bolt() {
    cylinder(d=6, h=4, $fn=24);
    cylinder(d=3.5, h=20, $fn=24);
}

mirror([0,0,1])
difference() {
    union() {
        difference() {
            translate([0,-yt-2*hr,0]) boxtop(xt,yt,fpt1);
            // Carve out interior
            translate([(xt+2*hr)/2,(-yt-2*hr)/2,fpt1/2+fpt2]) cube([xt,yt,fpt1], center=true);
        }
        m1x = b-2*hr+4;
        m1y = b+y1+y2;
        // side tabs
        translate([hr,-yt/2-hr,fpt2]) cube([2,10,fpt1-fpt2+1]);
        translate([xt+hr-2,-yt/2-hr,fpt2]) cube([2,10,fpt1-fpt2+1]);
        // top tab
        translate([xt/2-hr/2,-2-hr,fpt2]) cube([10,2,fpt1-fpt2+1]);
        // bottom tab
        translate([xt/2-8,-yt-hr,fpt2]) cube([10,2,fpt1-fpt2+1]);
    }
    translate([hr+2,-hr-2,0]) m3bolt();
    translate([xt+hr-2,-hr-2,0]) m3bolt();
    translate([xt/2+5.8,-yt-hr+2,0]) m3bolt();
    // blockouts to test
    //translate([0,-80,0]) cube([150,80,30]);
    //translate([70,-120,0]) cube([150,120,30]);
}

