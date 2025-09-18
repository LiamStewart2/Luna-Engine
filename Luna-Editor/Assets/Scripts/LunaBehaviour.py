class LunaBehaviour:
	def __init__(self, name):
		self.name = name

	def update(self, delta_time):
		print(f"[Python] {self.name} updating with dt = {delta_time}")