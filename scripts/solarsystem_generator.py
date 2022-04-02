from yattag import Doc, indent
import sys

factor = 60000
dist_scale = 3
output = sys.argv[1]
doc, tag, text = Doc().tagtext()

with tag('world'):
    with tag('camera'):
        doc.stag('position', x="100", y="100", z="100")
        doc.stag('lookAt', x="0", y="0", z="0")
        doc.stag('up', x="0", y="1", z="0")
        doc.stag('projection', fov="60", near="1", far="1000")
    with tag('group'):
        with tag('models'):
            pass
        with tag('group', name='sun'):
            with tag('models'):
                doc.stag('model', file='sphere.3d')
            with tag('transform'):
                size = dist_scale
                doc.stag('scale', x=str(size), y=str(size), z=str(size))
        with tag('group', name='mercury'):
            with tag('models'):
                doc.stag('model', file='sphere.3d')
            with tag('transform'):
                x_dist = 1 + dist_scale
                doc.stag('translate', x=str(x_dist), y=str(0), z=str(0))
                size = 2439.7 / factor
                doc.stag('scale', x=str(size), y=str(size), z=str(size))
        with tag('group', name='venus'):
            with tag('models'):
                doc.stag('model', file='sphere.3d')
            with tag('transform'):
                x_dist = 2 + dist_scale
                doc.stag('translate', x=str(x_dist), y=str(0), z=str(0))
                size = 6052 / factor
                doc.stag('scale', x=str(size), y=str(size), z=str(size))
        with tag('group', name='earth'):
            with tag('models'):
                doc.stag('model', file='sphere.3d')
            with tag('transform'):
                x_dist = 2.5 + dist_scale
                doc.stag('translate', x=str(x_dist), y=str(0), z=str(0))
                size = 6371 / factor
                doc.stag('scale', x=str(size), y=str(size), z=str(size))
            with tag('group', name='moon'):
                with tag('models'):
                    doc.stag('model', file='sphere.3d')
                with tag('transform'):
                    x_dist = size + 10
                    doc.stag('rotate', angle="35", x="1", y="0", z="1")
                    doc.stag('translate', x=str(x_dist), y=str(0), z=str(0))
                    doc.stag('scale', x=str(size), y=str(size), z=str(size))
        with tag('group', name='mars'):
            with tag('models'):
                doc.stag('model', file='sphere.3d')
            with tag('transform'):
                x_dist = 4 + dist_scale
                doc.stag('translate', x=str(x_dist), y=str(0), z=str(0))
                size = 3389.5 / factor
                doc.stag('scale', x=str(size), y=str(size), z=str(size))
        with tag('group', name='jupiter'):
            with tag('models'):
                doc.stag('model', file='sphere.3d')
            with tag('transform'):
                x_dist = 13 + dist_scale
                doc.stag('translate', x=str(x_dist), y=str(0), z=str(0))
                size = 69911 / factor
                doc.stag('scale', x=str(size), y=str(size), z=str(size))
        with tag('group', name='saturn'):
            with tag('models'):
                doc.stag('model', file='sphere.3d')
            with tag('transform'):
                x_dist = 24 + dist_scale
                doc.stag('translate', x=str(x_dist), y=str(0), z=str(0))
                size = 58232 / factor
                doc.stag('scale', x=str(size), y=str(size), z=str(size))
            with tag('group', name='ring'):
                with tag('models'):
                    doc.stag('model', file="torus.3d")
                with tag('transform'):
                    doc.stag('rotate', angle="35", x="1", y="0", z="0")
        with tag('group', name='uranus'):
            with tag('models'):
                doc.stag('model', file='sphere.3d')
            with tag('transform'):
                x_dist = 49 + dist_scale
                doc.stag('translate', x=str(x_dist), y=str(0), z=str(0))
                size = 25362 / factor
                doc.stag('scale', x=str(size), y=str(size), z=str(size))
        with tag('group', name='neptune'):
            with tag('models'):
                doc.stag('model', file='sphere.3d')
            with tag('transform'):
                x_dist = 76 + dist_scale
                doc.stag('translate', x=str(x_dist), y=str(0), z=str(0))
                size = 24662 / factor
                doc.stag('scale', x=str(size), y=str(size), z=str(size))

result = indent(
    doc.getvalue(),
    indentation = ' ' * 4,
    newline = '\n'
)

f = open(output, "w")
f.write(result)