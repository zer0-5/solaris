from yattag import Doc, indent
import sys
import csv
import random

factor = 60000
dist_scale = 3
output = sys.argv[1]
f_satelites = open("scripts/satelites.csv", "rt")
satelites = csv.DictReader(f_satelites)
f_planets = open("scripts/planets.csv", "rt")
planets = csv.DictReader(f_planets)
doc, tag, text = Doc().tagtext()
satelites_dic = {}

for satelite in satelites:
    planet = satelite["planet"]
    if planet in satelites_dic.keys():
        satelites_dic[planet].append(satelite)
    else:
        satelites_dic[planet] = [satelite]

with tag('world'):
    with tag('camera'):
        doc.stag('position', x="10", y="10", z="10")
        doc.stag('lookAt', x="0", y="0", z="0")
        doc.stag('up', x="0", y="1", z="0")
        doc.stag('projection', fov="60", near="1", far="1000")
    with tag('group'):
        with tag('models'):
            pass
        with tag('group', name='sun'):
            with tag('models'):
                doc.stag('model', file='models/sphere.3d')
            with tag('transform'):
                size = dist_scale
                doc.stag('scale', x=str(size), y=str(size), z=str(size))
        for planet in planets:
            name = planet["planet"]
            radius = float(planet["radius"]) / factor
            x_dist = float(planet["relative distance"]) + dist_scale
            with_ring = planet["has ring"] == "True"
            rot_angle = random.uniform(0, 360)
            with tag('group', name=name):
                with tag('models'):
                    doc.stag('model', file='models/sphere.3d')
                with tag('transform'):
                    doc.stag('rotate', angle=rot_angle, x=0, y=1, z=0)
                    doc.stag('translate', x=x_dist, y=0, z=0)
                    doc.stag('scale', x=radius, y=radius, z=radius)
                if with_ring:
                    with tag('group', name='ring'):
                        with tag('models'):
                            doc.stag('model', file='models/torus.3d')
                        with tag('transform'):
                            doc.stag('rotate', angle=35, x=1, y=0, z=0)
                if name in satelites_dic.keys():
                    satelites_planet = satelites_dic[name]
                    for sat in satelites_planet:
                        sat_name = sat["name"]
                        sat_radius = (float(sat["radius"]) / factor) / radius
                        with tag('group', name=sat_name):
                            with tag('models'):
                                doc.stag('model', file='models/sphere.3d')
                            with tag('transform'):
                                dist = random.uniform(1.5 * radius, 2.5 * radius) / radius
                                rot_angle = random.uniform(0, 360)
                                doc.stag('rotate', angle=rot_angle, x=0, y=1, z=0)
                                doc.stag('translate', x=dist, y=0, z=0)
                                doc.stag('scale', x=sat_radius, y=sat_radius, z=sat_radius)
        with tag('group', name="Asteroid Belt"):
            with tag('models'):
                pass
            for x in range(1, 200):
                with tag ('group', name="Asteroid" + f"{x}"):
                    with tag('models'):
                        doc.stag('model', file='models/sphere.3d')
                    with tag('transform'):
                        dist = random.uniform(dist_scale + 8, dist_scale + 9)
                        size = random.uniform(0.01, 0.03)
                        rot_angle = random.uniform(0, 360)
                        doc.stag('rotate', angle=rot_angle, x=0, y=1, z=0)
                        doc.stag('translate', x=dist, y = 0, z = 0)
                        doc.stag('scale', x=size, y=size, z=size)
                    
result = indent(
    doc.getvalue(),
    indentation = ' ' * 4,
    newline = '\n'
)

f = open(output, "w")
f.write(result)