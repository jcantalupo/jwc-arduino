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

fpt1 = 19; // front piece thickness 18 should be ok
fpt2 = 1; // front plate thickness

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

module battery9v() {
    color("blue",.5) cube([17,26,45+5]);
}

module usbc() {
    translate([0,4,0]) cylinder(d=8, h=15, $fn=32);
    translate([0,0,7.5]) cube([8,8,15], center=true);
    translate([0,-4,0]) cylinder(d=8, h=15, $fn=32);
}

module usbc2() {
    translate([0,3,0]) cylinder(d=8, h=15, $fn=32);
    translate([0,0,7.5]) cube([8,6,15], center=true);
    translate([0,-3,0]) cylinder(d=8, h=15, $fn=32);
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

module btn() {
    cylinder(d=bd, h=100, $fn=32, center=true);
}

module scrm() {
    cylinder(d=scr, h=100, $fn=32);
}

module led() {
    cylinder(d=5.1, h=100, $fn=32, center=true);
}

module led3mm() {
    cylinder(d=3.1, h=100, $fn=32, center=true);
}

module switch() {
    cylinder(d=.25*c, h=100, $fn=32, center=true);
}

module pot() {
    cylinder(d=(9/32)*c, h=100, $fn=32, center=true);
    translate([8,0,0]) cube([1.2,2.7,100],center=true);
}

module wedge() {
    difference() {
        cube([25,10,17]);
        translate([0,11,0]) rotate([30,0,0]) cube([25,10,37]);
    }
}

module slideSwitch() {
    cube([9,5,6], center=true);
    translate([19/2,0,0]) cylinder(h=6, d=2.5, center=true, $fn=16);
    translate([-19/2,0,0]) cylinder(h=6, d=2.5, center=true, $fn=16);
}


difference() {
    union() {
        difference() {
            translate([0,-yt-2*hr,0]) boxtop(xt,yt,fpt1);
            // Carve out interior
            translate([(xt+2*hr)/2,(-yt-2*hr)/2,fpt1/2+fpt2]) cube([xt,yt,fpt1], center=true);
        }
        // Add shelves for circuit boards
        m1x = b-2*hr+4;
        m1y = b+y1+y2;
        // sides
        translate([0,-m1y-2*hr,fpt2]) boxtop(m1x,m1y,bbh);
        translate([xt-b/2-.4,-m1y-2*hr,fpt2]) boxtop(m1x,m1y,bbh);
        // top
        //translate([hr,-b,fpt2]) cube([xt,b,bbh]);
        // ribs
        rw = .1*c;
        r1 = y2 + y3;
        translate([x1 + 1.5*c,-r1,fpt2]) cube([rw,r1,bbh]);
        translate([x1 + 2.2*c,-r1,fpt2]) cube([rw,r1,bbh]);
        translate([x1 + 1.55*c,-b-.6*c,fpt2]) cylinder(d=.2*c+1, h=bbh, $fn=24);
        translate([x1 + 2.25*c,-b-.6*c,fpt2]) cylinder(d=.2*c+1, h=bbh, $fn=24);
        // Nano
        translate([hr,-yt-hr,fpt2]) cube([59,8,nh]);
        translate([hr,-yt-hr+16,fpt2]) cube([59,8,nh]);
        // battery
        translate([hr+xt-38,-yt-hr+26,fpt2]) wedge();
    }
    // screw mount holes
    translate([b,-b,fpt2]) scrm();
    translate([b+x1+4*x2+x1,-b,fpt2]) scrm();
    // Top row buttons
    translate([b+x1,-b-y1,0]) btn();
    translate([b+x1+x2,-b-y1,0]) btn();
    translate([b+x1+2*x2,-b-y1,0]) btn();
    translate([b+x1+3*x2,-b-y1,0]) btn();
    translate([b+x1+4*x2,-b-y1,0]) btn();
    // middle screw mount holes
    translate([x1 + 1.55*c,-b-.6*c,fpt2]) scrm();
    translate([x1 + 2.25*c,-b-.6*c,fpt2]) scrm();
    // Nono screw mount holes
    translate([hr+7,-yt-hr+4,fpt2]) scrm();
    translate([hr+7+1.4*c,-yt-hr+4,fpt2]) scrm();
    translate([hr+7+1.7*c,-yt-hr+4+.6*c,fpt2]) scrm();
    // Second row buttons
    translate([b+x1+x2,-b-y1-y2,0]) btn();
    translate([b+x1+2*x2,-b-y1-y2,0]) btn();
    translate([b+x1+3*x2,-b-y1-y2,0]) btn();
    // Second row LEDs
    translate([b+x1,-b-y1-y2,0]) led();
    translate([b+x1+4*x2,-b-y1-y2,0]) led();
    // screw mount holes
    translate([b,-b-y3,fpt2]) scrm();
    translate([b+x1+4*x2+x1,-b-y3,fpt2]) scrm();
    // Switches and pots
    translate([b+x1,-b-y1-y2-y4,0]) pot();
    translate([b+x1+x2,-b-y1-y2-y4,0]) switch();
    translate([b+x1+2*x2,-b-y1-y2-y4,0]) led3mm();
    translate([b+x1+3*x2,-b-y1-y2-y4,0]) switch();
    translate([b+x1+4*x2,-b-y1-y2-y4,0]) pot();
    // power
    translate([b+x1+2*x2,-b-y1-y2-y4-10,0]) slideSwitch();
    // USB-C
    //translate([-5,-yt-hr+18,7+nh+fpt2]) rotate([0,90,0]) usbc();
    translate([-5,-yt-hr+18+1,7+nh+fpt2]) rotate([0,90,0]) usbc2();
    // blockouts to test Nano/battery half
    //translate([0,-80,0]) cube([150,80,30]);
    //translate([70,-120,0]) cube([150,120,30]);
}
// M3 inserts to screw on back
translate([hr+2,-hr-2,0]) m3i(fpt1);
translate([xt+hr-2,-hr-2,0]) m3i(fpt1);
translate([xt/2+5.8,-yt-hr+2,0]) m3i(fpt1);


// 9V battery - comment this out
//translate([xt-50+hr,-yt-hr,fpt2+17]) rotate([0,90,0]) battery9v();

