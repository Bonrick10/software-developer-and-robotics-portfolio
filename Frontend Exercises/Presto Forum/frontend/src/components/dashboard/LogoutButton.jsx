import React from 'react';
import axios from 'axios';
import { Button } from 'react-bootstrap';

import { AuthContext } from '../../contexts/AuthContext';

export function LogoutButton ({ setLocalToken }) {
  const { config } = React.useContext(AuthContext);

  const logout = async () => {
    try {
      await axios.post('http://localhost:5005/admin/auth/logout', {}, config);
    } catch (err) {
      alert(err.response.data.error);
    }
    setLocalToken('null');
  };
  return (
    <Button variant="danger" onClick={logout}>
      Logout
    </Button>
  );
}
