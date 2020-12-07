def material_print(row, col, fout):
    fout.write('    material:\n')
    if (row + col) % 2 == 0:
        fout.write('      diffuse: [0, 0, 0]\n')
        fout.write('      specular: [0, 0, 0]\n')
    else:
        fout.write('      diffuse: [1, 1, 1]\n')
        fout.write('      specular: [0, 0, 0]\n')
    
    fout.write('      shininess: 0\n')
    fout.write('      reflection: 0\n')
    fout.write('      refraction: 0\n')
    fout.write('      transparent: false\n')


with open('cells.yml', 'w') as fout:
    fout.write('objects:\n')

    for row in range(20):
        for col in range(20):
            fout.write('  cell_' + str(row) + '_' + str(col) + '_1:\n')
            fout.write('    type: triangle\n')
            fout.write('    p1: [' + str(row * 10) + ', ' + str(col * 10) + ', 0]\n')
            fout.write('    p2: [' + str(row * 10 + 10) + ', ' + str(col * 10) + ', 0]\n')
            fout.write('    p3: [' + str(row * 10) + ', ' + str(col * 10 + 10) + ', 0]\n')
            material_print(row, col, fout)

            fout.write('  cell_' + str(row) + '_' + str(col) + '_2:\n')
            fout.write('    type: triangle\n')
            fout.write('    p1: [' + str(row * 10 + 10) + ', ' + str(col * 10 + 10) + ', 0]\n')
            fout.write('    p2: [' + str(row * 10 + 10) + ', ' + str(col * 10) + ', 0]\n')
            fout.write('    p3: [' + str(row * 10) + ', ' + str(col * 10 + 10) + ', 0]\n')
            material_print(row, col, fout)

    fout.write('lights:\n')
    fout.write('  light:\n')
    fout.write('    position: [100, 100, 100]\n')
    fout.write('    color: [1, 1, 1]\n')
    fout.write('    falloff: [1, 0, 0]\n')

    fout.write('observer:\n')
    fout.write('  eye: [100, 100, 100]\n')
    fout.write('  ambient: [0.1, 0.1, 0.1]\n')
    fout.write('  up: [0, 1, 0]\n')
    fout.write('  view: [0, 0, -1]\n')
    fout.write('  fovy: 60\n')

    fout.write('space:\n')
    fout.write('  x: [-1, 201]\n')
    fout.write('  y: [-1, 201]\n')
    fout.write('  z: [-1, 101]\n')