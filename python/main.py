from models.gan3d import Discriminator3d
import dummy

data = dummy.generate_data(5, (64, 64, 64))
k = Discriminator3d(True, 5, 0.001, 64, 15)

k.predict(data)