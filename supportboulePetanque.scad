difference(){
cube([80,80,20], center=true);
translate([0,0,27]){
sphere(d=70.2, $fn=200);
}
cylinder(30,2,2,center=true);
}