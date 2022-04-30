import math

class CurvePoints:
    def __init__(self, n_divs, radius):
        self.n_divs = n_divs
        self.radius = radius

    def calc_points(self):
        step = int(90 / self.n_divs)
        list_points = []
        for alpha in range(360, 0, -step):
            x = self.radius * math.cos(math.radians(alpha))
            z = self.radius * math.sin(math.radians(alpha))
            list_points.append({"x":x, "y":0, "z":z})
        return list_points
