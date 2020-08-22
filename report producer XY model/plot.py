# =============================================================================
# =============================================================================
#  2D Ising model -report 
#  =============================================================================
# Updated on Aug 22 2020
# @author: Ahmed Alkharusi
# =============================================================================
import matplotlib.pyplot as plt
import numpy as np
pi = 3.14159265359

heat_capacity = np.loadtxt("results//heat_capacity.txt")
absolute_mag = np.loadtxt("results//absolute_magnetization_vec.txt")
energies= np.loadtxt("results//energies.txt")
temperatures = np.loadtxt("results//temperatures.txt")
lattice = np.loadtxt("results//lattice.txt")
lattice = lattice * (2.0*pi)/1000.0


X, Y = np.meshgrid(np.arange(0, lattice.shape[0], 1), np.arange(0,lattice.shape[0], 1))
u = np.cos(lattice)
v = np.sin(lattice)
fig, ax = plt.subplots()
q = ax.quiver(X,Y,u,v)

plt.tick_params(axis='both',which='both',
bottom=False,
top=False, 
left=False, 
right=False,   
labelbottom=False,
labeltop=False,
labelleft=False,
labelright=False)
plt.imshow(lattice, cmap="hsv")
plt.savefig("lattice.pdf")
plt.savefig("lattice.png",dpi=1000)
plt.show()


critical_temperature_index = np.argwhere(np.round(temperatures,2)==1.2)[0]

plt.rcParams.update({'font.size': 14})

fig,ax0 = plt.subplots()

ax0.set_xlabel("Temperature (K)")
ax0.set_ylabel("Heat Capacity per site (arb.)")

ax0.set_ylim(0,2*abs(heat_capacity[critical_temperature_index]))


ax0.scatter(temperatures,abs(heat_capacity))
ax0.grid()

ax0.set_title("Metropolis algorithm")
plt.savefig("Metropolis_report//heat_capacity.png",bbox_inches='tight',dpi=600)
plt.savefig("Metropolis_report//heat_capacity.pdf",bbox_inches='tight')

plt.show()




fig,ax1 = plt.subplots()

ax1.set_xlabel("Temperature (K)")
ax1.set_ylabel("Absolute Magnetization (arb.)")
ax1.scatter(temperatures,absolute_mag)
ax1.grid()
ax1.set_title("Metropolis algorithm")
plt.savefig("Metropolis_report//AbsoluteMagnetization.png",bbox_inches='tight',dpi=600)
plt.savefig("Metropolis_report//AbsoluteMagnetization.pdf",bbox_inches='tight')

plt.show()



fig,ax2 = plt.subplots()

ax2.set_xlabel("Temperature (K)")
ax2.set_ylabel("Energy (arb.)")
ax2.scatter(temperatures,energies)
ax2.grid()
ax2.set_title("Metropolis algorithm")
plt.savefig("Metropolis_report//Energy.png",bbox_inches='tight',dpi=600)
plt.savefig("Metropolis_report//Energy.pdf",bbox_inches='tight')

plt.show()



