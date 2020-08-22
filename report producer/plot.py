# =============================================================================
# =============================================================================
#  2D Ising model -report
#  =============================================================================
# Updated on Aug 22 2020
# @author: Ahmed Alkharusi
# =============================================================================
import matplotlib.pyplot as plt
import numpy as np
metropolis = True #false for Wolff alogarithm
heat_capacity = np.loadtxt("heat_capacity.txt")
absolute_mag = np.loadtxt("absolute_magnetization_vec.txt")
energies= np.loadtxt("energies.txt")
temperatures = np.loadtxt("temperatures.txt")

if metropolis==True:
    critical_temperature_index = np.argwhere(np.round(temperatures,2)==1.2)[0]

plt.rcParams.update({'font.size': 14})

fig,ax0 = plt.subplots()

ax0.set_xlabel("Temperature (K)")
ax0.set_ylabel("Heat Capacity per site (arb.)")
if metropolis==True:
    ax0.set_ylim(0,2*abs(heat_capacity[critical_temperature_index]))


ax0.scatter(temperatures,abs(heat_capacity))
ax0.grid()
if metropolis==True:
    ax0.set_title("Metropolis algorithm")
    plt.savefig("Metropolis_report//heat_capacity.png",bbox_inches='tight',dpi=600)
    plt.savefig("Metropolis_report//heat_capacity.pdf",bbox_inches='tight')
else:
    ax0.set_title("Wolff algorithm")
    plt.savefig("Wolff_report//heat_capacity.png",bbox_inches='tight',dpi=600)
    plt.savefig("Wolff_report//heat_capacity.pdf",bbox_inches='tight')
plt.show()




fig,ax1 = plt.subplots()

ax1.set_xlabel("Temperature (K)")
ax1.set_ylabel("Absolute Magnetization (arb.)")
ax1.scatter(temperatures,absolute_mag)
ax1.grid()
if metropolis==True:
    ax1.set_title("Metropolis algorithm")
    plt.savefig("Metropolis_report//AbsoluteMagnetization.png",bbox_inches='tight',dpi=600)
    plt.savefig("Metropolis_report//AbsoluteMagnetization.pdf",bbox_inches='tight')
else:
    ax1.set_title("Wolff algorithm")
    plt.savefig("Wolff_report//AbsoluteMagnetization.png",bbox_inches='tight',dpi=600)
    plt.savefig("Wolff_report//AbsoluteMagnetization.pdf",bbox_inches='tight')
plt.show()



fig,ax2 = plt.subplots()

ax2.set_xlabel("Temperature (K)")
ax2.set_ylabel("Energy (arb.)")
ax2.scatter(temperatures,energies)
ax2.grid()
if metropolis==True:
    ax2.set_title("Metropolis algorithm")
    plt.savefig("Metropolis_report//Energy.png",bbox_inches='tight',dpi=600)
    plt.savefig("Metropolis_report//Energy.pdf",bbox_inches='tight')
else:
    ax2.set_title("Wolff algorithm")
    plt.savefig("Wolff_report//Energy.png",bbox_inches='tight',dpi=600)
    plt.savefig("Wolff_report//Energy.pdf",bbox_inches='tight')
plt.show()
