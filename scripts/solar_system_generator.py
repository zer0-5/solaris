from yattag import Doc, indent
from PIL import ImageColor
from calc_curve_points import CurvePoints
import sys
import csv
import random

factor = 60000
dist_scale = 3
n_divs_curve = 10
output = sys.argv[1]
f_satellites = open("scripts/satellites.csv", "rt")
satellites = csv.DictReader(f_satellites)
f_planets = open("scripts/planets.csv", "rt")
planets = csv.DictReader(f_planets)
doc, tag, text = Doc().tagtext()
satellites_dic = {}

for satellite in satellites:
    planet = satellite["planet"]
    if planet in satellites_dic.keys():
        satellites_dic[planet].append(satellite)
    else:
        satellites_dic[planet] = [satellite]

with tag('world'):
    with tag('camera'):
        doc.stag('position', x="10", y="10", z="10")
        doc.stag('lookAt', x="0", y="0", z="0")
        doc.stag('up', x="0", y="1", z="0")
        doc.stag('projection', fov="60", near="1", far="1000")
    with tag('group'):
        with tag('group', name='Sun'):
            (r, g, b) = ImageColor.getcolor("#FDB813", "RGB")
            with tag('models'):
                with tag('model', file='models/sphere.3d'):
                    doc.stag('color', r=r, g=g, b=b)
            with tag('transform'):
                size = dist_scale
                doc.stag('scale', x=str(size), y=str(size), z=str(size))
        for planet in planets:
            name = planet["planet"]
            radius = float(planet["radius"]) / factor
            x_dist = float(planet["relative distance"]) + dist_scale
            with_ring = planet["has ring"] == "True"
            orbit = int(planet["orbit time (days)"]) / 40
            rot_angle = random.uniform(0, 360)
            rot_time = int(planet["rotation time (minutes)"]) / 40
            curve_points = CurvePoints(n_divs_curve, x_dist).calc_points()
            (r, g, b) = ImageColor.getcolor(planet["color"], "RGB")
            with tag('group', name=name):
                if with_ring:
                    with tag('group', name="Planet"):
                        with tag('models'):
                            with tag('model', file='models/sphere.3d'):
                                doc.stag('color', r=r, g=g, b=b)
                        with tag('transform'):
                            doc.stag('rotate', angle=rot_angle, x=0, y=1, z=0)
                            with tag('translate', time=orbit, align=False):
                                for point in curve_points:
                                    doc.stag('point', x=point["x"], y=point["y"], z=point["z"])
                            if name == "Saturn":
                                doc.stag('rotate', angle=45, x=1, y=0, z=0)
                            else:
                                doc.stag('rotate', angle=70, x=1, y=0, z=0)
                            doc.stag('rotate', time=rot_time, x=0, y=1, z=0)
                    with tag('group', name="Ring"): 
                        if name == "Saturn":
                            (r, g, b) = ImageColor.getcolor("#ab604a", "RGB")
                        else:
                            (r, g, b) = ImageColor.getcolor("#7ea7a8", "RGB")
                        with tag('group', name='ring'):
                            with tag('models'):
                                with tag('model', file='models/torus.3d'):
                                    doc.stag('color', r=r, g=g, b=b)
                            with tag('transform'):
                                doc.stag('rotate', angle=rot_angle, x=0, y=1, z=0)
                                with tag('translate', time=orbit, align=False):
                                    for point in curve_points:
                                        doc.stag('point', x=point["x"], y=point["y"], z=point["z"])
                                if name == "Saturn":
                                    doc.stag('rotate', angle=45, x=1, y=0, z=0)   
                                else:
                                    doc.stag('rotate', angle=70, x=1, y=0, z=0)
                else:
                    with tag('models'):
                        with tag('model', file='models/sphere.3d'):
                            doc.stag('color', r=r, g=g, b=b)
                    with tag('transform'):
                        doc.stag('rotate', angle=rot_angle, x=0, y=1, z=0)
                        with tag('translate', time=orbit, align=False):
                            for point in curve_points:
                                doc.stag('point', x=point["x"], y=point["y"], z=point["z"])
                        doc.stag('rotate', time=rot_time, x=0, y=1, z=0)
                        doc.stag('scale', x=radius, y=radius, z=radius)

                if name in satellites_dic.keys():
                    satellites_planet = satellites_dic[name]
                    for sat in satellites_planet:
                        sat_name = sat["name"]
                        sat_radius = (float(sat["radius"]) / factor) / radius
                        (sat_r, sat_g, sat_b) = ImageColor.getcolor("#767676", "RGB")
                        with tag('group', name=sat_name):
                            with tag('models'):
                                with tag('model', file='models/sphere_low_res.3d'):
                                    doc.stag('color', r=sat_r, g=sat_g, b=sat_b)
                            with tag('transform'):
                                dist = random.uniform(1.5 * radius, 2.5 * radius) / radius
                                rot_angle = random.uniform(0, 360)
                                curve_points = CurvePoints(n_divs_curve, dist).calc_points()
                                doc.stag('rotate', angle=rot_angle, x=0, y=1, z=0)
                                with tag('translate', time=random.uniform(30, 50), align=False):
                                    for point in curve_points:
                                        doc.stag('point', x=point["x"], y=point["y"], z=point["z"])
                                doc.stag('scale', x=sat_radius, y=sat_radius, z=sat_radius)
        with tag('group', name="Asteroid Belt"):
            (r, g, b) = ImageColor.getcolor("#5a554c", "RGB")
            curve_points = CurvePoints(n_divs_curve, random.uniform(dist_scale + 8, dist_scale + 9)).calc_points()
            for x in range(1, 501):
                with tag ('group', name="Asteroid" + f"{x}"):
                    with tag('models'):
                        with tag('model', file='models/sphere_low_res.3d'):
                            doc.stag('color', r=r, g=g, b=b)
                    with tag('transform'):
                        size = random.uniform(0.01, 0.03)
                        rot_angle = random.uniform(0, 360)
                        doc.stag('rotate', angle=rot_angle, x=0, y=1, z=0)
                        with tag('translate', time=random.uniform(50, 70), align=False):
                            for point in curve_points:
                                doc.stag('point', x=point["x"], y=point["y"], z=point["z"])
                        doc.stag('scale', x=size, y=size, z=size)

result = indent(
    doc.getvalue(),
    indentation = ' ' * 4,
    newline = '\n'
)

f = open(output, "w")
f.write(result)
