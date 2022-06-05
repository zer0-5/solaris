from yattag import Doc, indent
from PIL import ImageColor
from calc_curve_points import CurvePoints
import sys
import csv
import random
from math import sqrt, tan, pi

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
    with tag('lights'):
        doc.stag('light', type="point", posx="0", posy="0", posz="0")
    with tag('group'):
        with tag('group', name='Skybox'):
            with tag('models'):
                with tag('model', file='models/sphere.3d'):
                    doc.stag('texture', file="textures/8k_skybox.jpg")
                    with tag('color'):
                        doc.stag('ambient', R="255", G="255", B="255")
            with tag('transform'):
                doc.stag('scale', x="-500", y="-500", z="-500")
                doc.stag('rotate', angle="180", x="1", y="0", z="0")
        with tag('group', name='Sun'):
            (r, g, b) = ImageColor.getcolor("#FDB813", "RGB")
            with tag('models'):
                with tag('model', file='models/sphere.3d'):
                    doc.stag('texture', file="textures/8k_sun.jpg")
                    with tag('color'): # TODO: FIX VALUES
                        doc.stag('diffuse', R="200", G="200", B="200")
                        doc.stag('ambient', R="50", G="50", B="50")
                        doc.stag('specular', R="0", G="0", B="0")
                        doc.stag('emissive', R="255", G="255", B="255")
                        doc.stag('shininess', value="0")
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
            tilt = float(planet["axial tilt"])
            curve_points = CurvePoints(n_divs_curve, x_dist).calc_points()
            (r, g, b) = ImageColor.getcolor(planet["color"], "RGB")
            texture_file = planet["texture"]
            with tag('group', name=name):
                with tag('models'):
                    with tag('model', file='models/sphere.3d'):
                        doc.stag('texture', file=texture_file)
                        with tag('color'):
                            doc.stag('diffuse', R="200", G="200", B="200")
                            doc.stag('ambient', R="50", G="50", B="50")
                            doc.stag('specular', R="0", G="0", B="0")
                            doc.stag('emissive', R="0", G="0", B="0")
                            doc.stag('shininess', value="0")
                with tag('transform'):
                    doc.stag('rotate', angle=rot_angle, x=0, y=1, z=0)
                    with tag('translate', time=orbit, align=False):
                        for point in curve_points:
                            doc.stag('point', x=point["x"], y=point["y"], z=point["z"])
                    doc.stag('rotate', angle=tilt, x=1, y=0, z=0)
                    doc.stag('rotate', time=rot_time, x=0, y=1, z=0)
                    doc.stag('scale', x=radius, y=radius, z=radius)
                if with_ring:
                    with tag('group', name='ring'):
                        with tag('models'):
                            with tag('model', file='models/ring.3d'):
                                if name == "Uranus":
                                    doc.stag('texture', file='textures/uranus_ring.png')
                                else:
                                    doc.stag('texture', file='textures/8k_saturn_ring.png')
                                with tag('color'):
                                    doc.stag('diffuse', R="200", G="200", B="200")
                                    doc.stag('ambient', R="50", G="50", B="50")
                                    doc.stag('specular', R="0", G="0", B="0")
                                    doc.stag('emissive', R="0", G="0", B="0")
                                    doc.stag('shininess', value="0")
                if name in satellites_dic.keys():
                    satellites_planet = satellites_dic[name]
                    for sat in satellites_planet:
                        sat_name = sat["name"]
                        sat_radius = (float(sat["radius"]) / factor) / radius
                        (sat_r, sat_g, sat_b) = ImageColor.getcolor("#767676", "RGB")
                        with tag('group', name=sat_name):
                            with tag('models'):
                                with tag('model', file='models/sphere_lowres.3d'):
                                    doc.stag('texture', file="textures/8k_moon.jpg")
                                    with tag('color'):
                                        doc.stag('diffuse', R="200", G="200", B="200")
                                        doc.stag('ambient', R="50", G="50", B="50")
                                        doc.stag('specular', R="0", G="0", B="0")
                                        doc.stag('emissive', R="0", G="0", B="0")
                                        doc.stag('shininess', value="0")   
                            with tag('transform'):
                                dist = random.uniform(1.5 * radius, 2.5 * radius) / radius
                                rot_angle = random.uniform(0, 360)
                                curve_points = CurvePoints(n_divs_curve, dist).calc_points()
                                doc.stag('rotate', angle=rot_angle, x=1, y=0, z=0)
                                with tag('translate', time=random.uniform(5, 10), align=False):
                                    for point in curve_points:
                                        doc.stag('point', x=point["x"], y=point["y"], z=point["z"])
                                doc.stag('scale', x=sat_radius, y=sat_radius, z=sat_radius)
        with tag('group', name="Asteroid Belt"):
            (r, g, b) = ImageColor.getcolor("#5a554c", "RGB")
            for x in range(1, 501):
                curve_points = CurvePoints(n_divs_curve, random.uniform(dist_scale + 8, dist_scale + 9)).calc_points()
                with tag ('group', name=f"Asteroid {x}"):
                    with tag('models'):
                        with tag('model', file='models/sphere_lowres.3d'):
                            doc.stag('texture', file='textures/4k_haumea.jpg')
                            with tag('color'):
                                doc.stag('diffuse', R="200", G="200", B="200")
                                doc.stag('ambient', R="50", G="50", B="50")
                                doc.stag('specular', R="0", G="0", B="0")
                                doc.stag('emissive', R="0", G="0", B="0")
                                doc.stag('shininess', value="0")
                    with tag('transform'):
                        size = random.uniform(0.01, 0.03)
                        rot_angle = random.uniform(0, 360)
                        doc.stag('rotate', angle=rot_angle, x=0, y=1, z=0)
                        with tag('translate', time=random.uniform(50, 70), align=False):
                            for point in curve_points:
                                doc.stag('point', x=point["x"], y=point["y"], z=point["z"])
                        doc.stag('scale', x=size, y=size, z=size)
        with tag('group', name="Halley's Comet"):
            size = 0.1
            period = (76 * 365) / 40
            eccentricity = 0.976
            perihelion = 0.587 * (dist_scale + 10)
            aphelion = 35.3 * dist_scale

            a = (aphelion + perihelion) / (1 + eccentricity)
            b = a * sqrt(1 - eccentricity*eccentricity)
            (r, g, b) = ImageColor.getcolor("#5a554c", "RGB")
            with tag('models'):
                with tag('model', file='models/bezier.3d'):
                    doc.stag('texture', file='textures/teapot.jpg')
                    with tag('color'):
                        doc.stag('diffuse', R="200", G="200", B="200")
                        doc.stag('ambient', R="50", G="50", B="50")
                        doc.stag('specular', R="0", G="0", B="0")
                        doc.stag('emissive', R="0", G="0", B="0")
                        doc.stag('shininess', value="0")
            with tag('transform'):
                doc.stag('rotate', angle=-10, x=0, y=0, z=1)
                doc.stag('translate', x= aphelion * a / (perihelion + aphelion), y=0, z=0)
                with tag('translate', time=period, align=True):
                    for i in range(0, 360, n_divs_curve):
                        ang = i * (pi / 180)
                        bottom = sqrt(b*b + a*a*tan(ang)*tan(ang))
                        x = a * b / bottom
                        y = a * b * tan(ang) / bottom
                        if i > 90 and i <= 270:
                            x = -x
                            y = -y
                        doc.stag('point', x=x, y=0, z=y)
                doc.stag('rotate', angle=-90, x=1, y=0, z=0)
                doc.stag('scale', x=size, y=size, z=size)

result = indent(
    doc.getvalue(),
    indentation = ' ' * 4,
    newline = '\n'
)

f = open(output, "w")
f.write(result)
