import numpy as np
from scipy.integrate import odeint
import matplotlib.pyplot as plt
import imageio
import os

m_R = 0.1 #kg
m_l = 0.01
R = 0.2 #m
l = 0.8
I_R = 1/2*m_R*R**2
I_l = 1/12*m_l*l**2
tau = 0.005
g = 9.8 #gravitational acceleration

def eom(X, t):
    """
    theta_R, theta_Rdot, theta_l, theta_ldot
    """
    theta_R, theta_Rdot, theta_l, theta_ldot = X
    theta_Rdoubledot = -g*l*m_l*(l**2*m_l*np.sin(theta_R + theta_l)**2/4 - l*m_l*(-R - l*np.cos(theta_R + theta_l)/2)*np.cos(theta_R + theta_l)/2)*np.sin(theta_R + theta_l)/(2*(-(l**2*m_l*np.sin(theta_R + theta_l)**2/4 - l*m_l*(-R - l*np.cos(theta_R + theta_l)/2)*np.cos(theta_R + theta_l)/2)**2 + (I_l + l**2*m_l*np.sin(theta_R + theta_l)**2/4 + l**2*m_l*np.cos(theta_R + theta_l)**2/4)*(I_R + R**2*m_R + l**2*m_l*np.sin(theta_R + theta_l)**2/4 + m_l*(-R - l*np.cos(theta_R + theta_l)/2)**2))) + (-(-l**2*m_l*np.sin(theta_R + theta_l)**2/4 + l*m_l*(-R - l*np.cos(theta_R + theta_l)/2)*np.cos(theta_R + theta_l)/2)*(l**2*m_l*np.sin(theta_R + theta_l)**2/4 - l*m_l*(-R - l*np.cos(theta_R + theta_l)/2)*np.cos(theta_R + theta_l)/2) - (l**2*m_l*np.sin(theta_R + theta_l)**2/4 - l*m_l*(-R - l*np.cos(theta_R + theta_l)/2)*np.cos(theta_R + theta_l)/2)**2 + (I_l + l**2*m_l*np.sin(theta_R + theta_l)**2/4 + l**2*m_l*np.cos(theta_R + theta_l)**2/4)*(I_R + R**2*m_R + l**2*m_l*np.sin(theta_R + theta_l)**2/4 + m_l*(-R - l*np.cos(theta_R + theta_l)/2)**2))*(g*l*m_l*np.sin(theta_R + theta_l)/2 + tau + (-l**2*m_l*(theta_Rdot + theta_ldot)*np.sin(theta_R + theta_l)*np.cos(theta_R + theta_l)/4 + l*m_l*(R + l*np.cos(theta_R + theta_l)/2)*(theta_Rdot + theta_ldot)*np.sin(theta_R + theta_l)/2)*theta_Rdot + (-l**2*m_l*(theta_Rdot + theta_ldot)*np.sin(theta_R + theta_l)*np.cos(theta_R + theta_l)/4 + l*m_l*(R + l*np.cos(theta_R + theta_l)/2)*(theta_Rdot + theta_ldot)*np.sin(theta_R + theta_l)/2)*theta_ldot)/((-(l**2*m_l*np.sin(theta_R + theta_l)**2/4 - l*m_l*(-R - l*np.cos(theta_R + theta_l)/2)*np.cos(theta_R + theta_l)/2)**2 + (I_l + l**2*m_l*np.sin(theta_R + theta_l)**2/4 + l**2*m_l*np.cos(theta_R + theta_l)**2/4)*(I_R + R**2*m_R + l**2*m_l*np.sin(theta_R + theta_l)**2/4 + m_l*(-R - l*np.cos(theta_R + theta_l)/2)**2))*(I_R + R**2*m_R + l**2*m_l*np.sin(theta_R + theta_l)**2/4 + m_l*(-R - l*np.cos(theta_R + theta_l)/2)**2))
    theta_ldoubledot = g*l*m_l*(I_R + R**2*m_R + l**2*m_l*np.sin(theta_R + theta_l)**2/4 + m_l*(-R - l*np.cos(theta_R + theta_l)/2)**2)*np.sin(theta_R + theta_l)/(2*(-(l**2*m_l*np.sin(theta_R + theta_l)**2/4 - l*m_l*(-R - l*np.cos(theta_R + theta_l)/2)*np.cos(theta_R + theta_l)/2)**2 + (I_l + l**2*m_l*np.sin(theta_R + theta_l)**2/4 + l**2*m_l*np.cos(theta_R + theta_l)**2/4)*(I_R + R**2*m_R + l**2*m_l*np.sin(theta_R + theta_l)**2/4 + m_l*(-R - l*np.cos(theta_R + theta_l)/2)**2))) + (-l**2*m_l*np.sin(theta_R + theta_l)**2/4 + l*m_l*(-R - l*np.cos(theta_R + theta_l)/2)*np.cos(theta_R + theta_l)/2)*(g*l*m_l*np.sin(theta_R + theta_l)/2 + tau + (-l**2*m_l*(theta_Rdot + theta_ldot)*np.sin(theta_R + theta_l)*np.cos(theta_R + theta_l)/4 + l*m_l*(R + l*np.cos(theta_R + theta_l)/2)*(theta_Rdot + theta_ldot)*np.sin(theta_R + theta_l)/2)*theta_Rdot + (-l**2*m_l*(theta_Rdot + theta_ldot)*np.sin(theta_R + theta_l)*np.cos(theta_R + theta_l)/4 + l*m_l*(R + l*np.cos(theta_R + theta_l)/2)*(theta_Rdot + theta_ldot)*np.sin(theta_R + theta_l)/2)*theta_ldot)/(-(l**2*m_l*np.sin(theta_R + theta_l)**2/4 - l*m_l*(-R - l*np.cos(theta_R + theta_l)/2)*np.cos(theta_R + theta_l)/2)**2 + (I_l + l**2*m_l*np.sin(theta_R + theta_l)**2/4 + l**2*m_l*np.cos(theta_R + theta_l)**2/4)*(I_R + R**2*m_R + l**2*m_l*np.sin(theta_R + theta_l)**2/4 + m_l*(-R - l*np.cos(theta_R + theta_l)/2)**2))
    return theta_Rdot, theta_Rdoubledot, theta_ldot, theta_ldoubledot


tmax, dt = 3, 0.01
t = np.arange(0, tmax+dt, dt)
X0 = np.array([0, 0, np.pi/6, 0])

y = odeint(eom, X0, t)

theta_R, theta_l = y[:,0], y[:,2]

x_R1 = 0
x_R2 = -R*theta_R
x_l1 = -l/2*np.cos(theta_R + theta_l)
x_l2 = -R*theta_R - l/2*np.sin(theta_R + theta_l)

plt.plot(t, y)
plt.legend(['x_d1', 'x_d1dot', 'x_d2', 'x_d2dot', 'theta_d', 'theta_ddot', 'theta_p', 'theta_pdot'])
plt.show()

def make_plot(i):
    circle = plt.Circle((0, x_R2[i]), R, alpha = 0.5)
    ax.add_artist(circle)
    
    ax.plot([R*np.cos(theta_R[i]), 0, -R*np.sin(theta_R[i])],
            [x_R2[i] + R*np.sin(theta_R[i]), x_R2[i], x_R2[i] + R*np.cos(theta_R[i])])
    ax.plot([0, -l*np.cos(theta_R[i] + theta_l[i])],
             [-R*theta_R[i], -R*theta_R[i] - l*np.sin(theta_R[i] + theta_l[i])])
    
    lim = 2
    ax.set(xlim = [-lim, lim], ylim = [-lim, lim], title = 'wheel + body', xlabel = 'x', ylabel = 'y')
    ax.set_aspect('equal', adjustable='box')
    plt.axis('off')
    plt.savefig('_img{:04d}.png'.format(i//di), dpi=72)
    plt.cla()


rate = 60
di = int(1/rate/dt)
fig = plt.figure(figsize=(5, 5), dpi=72)
ax = fig.add_subplot(111)

for i in range(0, t.size, di):
    print(i // di, '/', t.size // di)
    make_plot(i)

png_dir = 'C:/Users/YangOng/Desktop/Bipedal Robot PID Control'
images = []
for filename in os.listdir(png_dir):
    if filename.endswith('.png'):
        file_path = os.path.join(png_dir, filename)
        images.append(imageio.imread(file_path))
imageio.mimsave('C:/Users/YangOng/Desktop/Bipedal Robot PID Control/anim.gif', images, fps=rate)
