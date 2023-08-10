```
################################################################################
# /etc/hosts for MMT MAPS
# Last updated 2023-08-10-09:45:00 MST
################################################################################

127.0.0.1      localhost localhost.localdomain localhost4 localhost4.localdomain4
::1            localhost localhost.localdomain localhost6 localhost6.localdomain6

# Network information:
#
# 1. LN - Lab Network.
# Within the second floor lab at Steward, there exists a network
# segment reserved for MAPS that is approximately 10.130.149.0/24.
# Accessing this network requires being connected to the Cisco VPN
# at https://vpn.arizona.edu/mmt-split or /mmt-full.
#
# 2. MUN - Mountain Unrouteable Network, aka "the rack network" or "the topbox network".
# At the MMT, there is also a network segment for 10.130.149.0/24. This network 
# is not routeable, and thus inaccessible unless you are physically at the MMT
# or use a computer that has an interface on a public, routeable network AND 
# a connection to the MAPS network segment. There is physical connection between
# the rack and the control room and a switch in the control room that you can 
# connect to that will give you access to this network. There is no DHCP on this
# network, so you should choose an address that doesn't conflict with others.
#
# 3. MRN - Mountain Routeable Network, aka "the building network".
# At the MMT, there is also a public, routeable network. IP addresses on this
# network are available globally without a VPN. IP addresses on this network
# are in 128.196.100.0/24 and may offer a bridge between the outside world and
# the MUN (see section 2).

# IP addresses that work in the lab with the VPN and in the 
# unrouteable network on the mountain.

10.130.149.2   switch switch.mmto.arizona.edu			    # The Cisco switch.
10.130.149.3   tb1 t620-topbox1.mmto.arizona.edu			#
10.130.149.4   tb2 t620-topbox2.mmto.arizona.edu			# 
10.130.149.5   modulator modulator.mmto.arizona.edu		    # 
10.130.149.6   acq acquisition-camera.mmto.arizona.edu		# Acquisition camera.
10.130.149.7   rc2-ipmi rc2-ipmi.mmto.arizona.edu		    # rc2 management IF.
10.130.149.8   irmon irmonitor.mmto.arizona.edu			    #
10.130.149.9   ircomp ircomp.mmto.arizona.edu			    #
10.130.149.10  rc3 p7920-rc3.mmto.arizona.edu 			    # rc3
10.130.149.11  rc4 p7920-rc4.mmto.arizona.edu			    # rc4
10.130.149.12  rc1 iris1291r-rc1.mmto.arizona.edu		    # rc1
10.130.149.13  rc2 iris1291r-rc2.mmto.arizona.edu		    # rc2
10.130.149.14  aopdu3 aopdu3.mmto.arizona.edu			    #
10.130.149.15  aopdu4 aopdu4.mmto.arizona.edu			    #
10.130.149.16  aopdu5 aopdu5.mmto.arizona.edu		 	    #
10.130.149.17  aopdu6 aopdu6.mmto.arizona.edu			    #
10.130.149.18  aopdu7 aopdu7.mmto.arizona.edu			    #
10.130.149.19  aopdu8 aopdu8.mmto.arizona.edu			    #
10.130.149.20  aops1 aops1.mmto.arizona.edu			        #
10.130.149.21  idrac-38498V3 idrac-38498V3.mmto.arizona.edu	# rc3 management IF.
10.130.149.22  aopdu1 aopdu1.mmto.arizona.edu			    #
10.130.149.23  idrac-48498V3 idrac-48498V3.mmto.arizona.edu	# rc4 management IF.
10.130.149.24  aopdu2 aopdu2.mmto.arizona.edu			    #
10.130.149.25  aoups aoups.mmto.arizona.edu			        #

# IP addresses that only work in the unrouteable network at
# the mountain to which you must be physically connected. These IPs
# are allocated to other users on the UITS network in Tucson, so
# using these IP addresses at Steward will cause conflicts and make
# UITS folks unhappy. Please only use these at the mountain.

10.130.149.26  op1 p3460-op1.mmto.arizona.edu			    # Operator computer 1.
10.130.149.42  op2 p3460-op2.mmto.arizona.edu			    # Operator computer 2.
10.130.149.43  jess                                         # Jess’s laptop in the control room.

# Public, routeable IP addresses that only work when the 
# machine in question is at the mountain and connected to
# the building network.

128.196.100.63 rc2-public

```